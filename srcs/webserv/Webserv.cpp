/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/25 21:30:13 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object manipulation ] -----------------------
*/
Webserv::Webserv() : _epoll_fd( -1 ) {}

Webserv::~Webserv()
{
	std::map<int, Client>::iterator cit = _clients.begin();

	for (; cit != _clients.end(); ++cit)
		::close( cit->first );
	_clients.clear();

	for (size_t i = 0; i < _listeners.size(); ++i)
		_listeners[i].closeFd();
	_listeners.clear();

	if (_epoll_fd != -1)
		::close( _epoll_fd );
}

/*
	----------------------------- [ Print ] ------------------------------
*/
static void		__requestReceived( Request& request )
{
	std::cout << P_YELLOW "\n--- Request received ---" NC << std::endl;
	std::cout << BOLD PINK "Method: " NC << request.getMethod() << std::endl;
	std::cout << BOLD PINK "URI: " NC << request.getUri() << std::endl;
	std::cout << BOLD PINK "Headers:\n" NC << request.getHeaderMap() << std::endl;
	std::cout << P_YELLOW "------------------------\n" NC << std::endl;
}

/*
	------------------------ [ P: Epoll events ] -------------------------
*/
bool	Webserv::_addServerToEpoll( int server_fd )
{
	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = server_fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev ) == -1)
	{
		err_msg( "epoll_ctl(ADD server)", strerror( errno ) );
		return (false);
	}
	return (true);
}

bool	Webserv::_addClientToEpoll( int client_fd )
{
	epoll_event ev;
	std::memset( &ev, 0, sizeof( ev ) );
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = client_fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, client_fd, &ev ) == -1)
	{
		err_msg( "epoll_ctl(ADD client)", strerror( errno ) );
		return (false);
	}
	return (true);
}

void	Webserv::_removeClient( int client_fd )
{
	epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, client_fd, NULL );
	::close( client_fd );

	std::cout << P_BLUE "[INFO] " NC "Client closed (fd=" << client_fd << ")" << std::endl;

	_clients.erase( client_fd );
}

bool	Webserv::_modifyEpollEvents( int fd, unsigned int events )
{
	epoll_event ev;
	std::memset( &ev, 0, sizeof(ev) );
	ev.events = events;
	ev.data.fd = fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_MOD, fd, &ev ) == -1)
	{
		err_msg( "epoll_ctl(MODIFY)", strerror( errno ) );
		return (false);
	}
	return (true);
}

/*
	--------------- [ P: Client & Server events handling ] ---------------
*/
void	Webserv::_handleListenerEvent( Listener& listener )
{
	while (true)
	{
		int client_fd = listener.acceptClient();
		if (client_fd == -1)
			break ;

		if (!_addClientToEpoll( client_fd ))
		{
			::close( client_fd );
			continue ;
		}

		Client new_client( client_fd, listener.socket_fd );
		_clients.insert( std::make_pair( client_fd, new_client ) );

		std::cout << P_BLUE "[INFO] " NC "Client accepted (fd=" << client_fd << ")" << std::endl; 
	}
}

void	Webserv::_handleClientEvent( int client_fd, unsigned int events )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	if (events & EPOLLERR)
		return (_removeClient( client_fd ));
	if (events & EPOLLIN)
		_handleClientRead( client_fd );
	if (events & EPOLLOUT)
		_handleClientWrite( client_fd );
}

void	Webserv::_handleClientRead( int client_fd )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	Client& client = it->second;
	char buffer[8192];

	while (true)
	{
		ssize_t nBytes = ::recv( client_fd, buffer, sizeof(buffer), 0 );
		if (nBytes < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			return (err_msg( "recv()", strerror( errno ) ));
		}
		if (nBytes == 0)
			return (_removeClient( client_fd ));

		client.appendToReadBuffer( buffer, nBytes );
	}
	client.updateLastActivity();

	if (client.hasCompleteRequest())
		_processRequest( client_fd );
}

void	Webserv::_handleClientWrite( int client_fd )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	Client& client = it->second;

	if (client.sendData())
	{
		if (!_modifyEpollEvents( client_fd, EPOLLIN ))
			return ;
		client.clearReadBuffer();
	}
}

Listener*	Webserv::_getListenerByFd( int fd )
{
	for (size_t i = 0; i < _listeners.size(); ++i)
	{
		if (_listeners[i].socket_fd == fd)
			return (&_listeners[i]);
	}
	return (NULL);
}

/*
	---------------------- [ P: Request handling ] ----------------------
*/
Response*	Webserv::_buildResponse( Request& request, Listener& listener )
{
	try {
		__requestReceived( request );

		std::string hostname = request.getHeaderValue( "Host" );
		ServerConfig server = listener.resolveVirtualHosting( hostname );

		// if (isCgiRequest(request, server) == true)
		// 	return (handleCgi());

		return (handleMethod( server, request ));
	}
	catch (const BadRequestException& e) {
		std::cerr << P_YELLOW "[DEBUG] " NC << e.what() << std::endl;
		return (handleHttpException( e ));
	}
	catch (const std::exception& e) {
		return (handleHttpException( e ));
	}
}

void	Webserv::_processRequest( int client_fd )
{
	std::map<int, Client>::iterator cit = _clients.find( client_fd );
	if (cit == _clients.end())
		return ;

	Client& client = cit->second;

	Listener* listener = _getListenerByFd( client.getListenerFd() );
	if (!listener)
	{
		err_msg( NULL, "Listener not found for client" );
		return (_removeClient( client_fd ));
	}

	Request request( client.getReadBuffer() );
	Response* response = _buildResponse( request, *listener );

	// check de la valeur du header "connection" et si c'est a "closed", je dois fermer la connexion
	std::string serialized = response->getSerializedResponse();
	delete response;

	client.appendToWriteBuffer( serialized );
	_modifyEpollEvents( client_fd, EPOLLIN | EPOLLOUT );
}

void	Webserv::_checkClientTimeout()
{
	std::map<int, Client>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if (it->second.isTimedOut( TIMEOUT ))
		{
			int client_fd = it->first;
			++it;
			std::cout << P_BLUE "[INFO] " NC "Client timeout (fd=" << client_fd << ")" << std::endl;
			_removeClient( client_fd );
		}
		else
			++it;
	}
}

/*
	------------------------- [ Public methods ] -------------------------
*/
void	Webserv::addServerConfig( ServerConfig& server )
{
	_servers.push_back( server );
}

bool	Webserv::initListeners()
{
	std::map<unsigned short, std::vector<ServerConfig*> > serversByPort;

	for (size_t i = 0; i < _servers.size(); ++i)
	{
		ServerConfig* server = &_servers[i];
		unsigned short port = server->getPort();

		serversByPort[port].push_back( server );
	}

	std::map<unsigned short, std::vector<ServerConfig*> >::iterator it = serversByPort.begin();

	for (; it != serversByPort.end(); ++it)
	{
		Listener listener;
		listener.socket_fd = -1;
		listener.port = it->first;
		listener.servers = it->second;

		if (!listener.init())
			return (false);

		_listeners.push_back( listener );
		listener.printInfo();
	}
	return (true);
}

bool	Webserv::init()
{
	_epoll_fd = epoll_create(1);
	if (_epoll_fd == -1)
	{
		err_msg( "epoll_create()", strerror( errno ) );
		return (false);
	}

	for (size_t i = 0; i < _listeners.size(); ++i)
	{
		if (!_addServerToEpoll( _listeners[i].socket_fd ))
			return (false);
	}
	return (true);
}

// handle signals & cleanup
void	Webserv::run()
{
	epoll_event events[MAX_EVENTS];

	std::cout << P_YELLOW "\nWaiting for new connections...\n" NC << std::endl;

	while (true)
	{
		int nbFds = epoll_wait( _epoll_fd, events, MAX_EVENTS, -1 );
		if (nbFds == -1)
		{
			if (errno == EINTR)
				continue ;
			err_msg( "epoll_wait()", strerror( errno ) );
			break ;
		}

		_checkClientTimeout();
		for (int i = 0; i < nbFds; ++i)
		{
			int fd = events[i].data.fd;
			Listener* listener = _getListenerByFd( fd );
			if (listener)
				_handleListenerEvent( *listener );
			else
				_handleClientEvent( fd, events[i].events );
		}
	}
}
