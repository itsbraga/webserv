/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/31 03:00:18 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Signal Handler ] -----------------------
*/
volatile sig_atomic_t g_stop = 0;

void	signal_handler( int __attribute__((unused))signo )
{
	g_stop = 1;
}

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
		_listeners[i].closeSocketFd();
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
	std::cout << BOLD PINK "Headers:\n" NC << request.printHeaderMap() << std::endl;
	std::cout << P_YELLOW "------------------------\n" NC << std::endl;
}

/*
	--------------------------- [ P: Setter ] ----------------------------
*/
bool	Webserv::setCloseOnExec( int fd )
{
	if (fcntl( fd, F_SETFD, FD_CLOEXEC ) == -1)
		return (err_msg( "fcntl(F_SETFD,  FD_CLOEXEC)", strerror( errno ) ), false);
	else
		return (true);
}

/*
	--------------------------- [ P: Getter ] ----------------------------
*/
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
Response*	Webserv::_executeRequest( int client_fd, Request& request, ServerConfig& server )
{
	__requestReceived( request );

	if (isReturn( request, server ))
		return (returnHandler( request, server ));
	else if (isCgiRequest( request, server ))
		return (cgiHandler( client_fd, request, server, (*this) ));
	else
		return (methodHandler( server, request ));
}

void	Webserv::_processRequest( int client_fd )
{
	std::map<int, Client>::iterator cit = _clients.find( client_fd );
	if (cit == _clients.end())
		return ;

	Client& client = cit->second;

	Listener* listener = _getListenerByFd( client.getListenerFd() );
	if (!listener)
		return (err_msg( NULL, "Listener not found" ), _removeClient( client_fd ));

	Response* response = NULL;
	bool close_client = false;

	try {
		Request request( client.getReadBuffer() );
		close_client = request.clientWantsClose();

		std::string hostname = request.getHeaderValue( "host" );
		ServerConfig server = listener->resolveVirtualHosting( hostname );

		response = _executeRequest( client_fd, request, server );
		if (response == NULL)
			return ;
		ErrorPageHandler( *response, request, server );
	}
	catch (const BadRequestException& e) {
		close_client = true;
		std::cerr << P_YELLOW "[DEBUG] " << e.what() << NC << std::endl;
		response = httpExceptionHandler( e );
	}
	catch (const ChildErrorException& e) {
		throw ;
	}
	catch (const std::exception& e) {
		close_client = true;
		response = httpExceptionHandler( e );
	}

	if (response->getHeaderValue( "connection" ) == "close")
		close_client = true;

	client.setShouldClose( close_client );

	std::string serialized = response->getSerializedResponse();
	delete response;

	client.appendToWriteBuffer( serialized );
	client.setResponsePending( true );
	_modifyEpollEvents( client_fd, EPOLLIN | EPOLLOUT );
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
	std::cout << BOLD PINK "=================== initListeners ===================\n" NC << std::endl;
	std::cout << "_servers.size() = " << _servers.size() << std::endl;

	std::map<unsigned short, std::vector<ServerConfig*> > serversByPort;

	for (size_t i = 0; i < _servers.size(); ++i)
	{
		ServerConfig* server = &_servers[i];
		unsigned short port = server->getPort();

		serversByPort[port].push_back( server );
	}
	std::cout << "Unique ports: " << serversByPort.size() << std::endl;

	std::map<unsigned short, std::vector<ServerConfig*> >::iterator it = serversByPort.begin();

	for (; it != serversByPort.end(); ++it)
	{
		std::cout << "Creating listener for port " << it->first 
				  << " with " << it->second.size() << " server(s)" << std::endl;

		Listener listener;
		listener.socket_fd = -1;
		listener.port = it->first;
		listener.servers = it->second;

		if (!listener.init())
			return (false);
		std::cout << "Listener created with fd=" << listener.socket_fd << std::endl;

		_listeners.push_back( listener );
		listener.printInfo();
	}

	std::cout << "Total listeners: " << _listeners.size() << "\n\n";
	std::cout << BOLD PINK "=====================================================\n" NC;
	return (true);
}

bool	Webserv::initEpoll()
{
	signal( SIGINT, signal_handler );
	signal( SIGTERM, signal_handler );
	signal( SIGPIPE, SIG_IGN );
	
	_epoll_fd = epoll_create(1);
	if (_epoll_fd == -1)
		return (err_msg( "epoll_create()", strerror( errno ) ), false);
	if (!setCloseOnExec(_epoll_fd))
	{
		::close(_epoll_fd);
		return (false);
	}

	for (size_t i = 0; i < _listeners.size(); ++i)
	{
		if (!_addServerToEpoll( _listeners[i].socket_fd ))
			return (false);
	}
	return (true);
}

void	Webserv::run()
{
	epoll_event events[MAX_EVENTS];

	std::cout << P_YELLOW "\nWaiting for new connections...\n" NC << std::endl;
	while (!g_stop)
	{
		int nbReady = epoll_wait( _epoll_fd, events, MAX_EVENTS, EPOLL_TIMEOUT );
		if (nbReady == -1)
		{
			if (errno == EINTR)
			{
				if (g_stop == 1)
					break;
				continue;
			}
			err_msg( "epoll_wait()", strerror( errno ) );
			break;
		}

		_checkCgiTimeout();
		_checkClientTimeout();

		for (int i = 0; i < nbReady; ++i)
		{
			if (g_stop == 1)
				break;

			int fd = events[i].data.fd;
			Listener* listener = _getListenerByFd( fd );

			if (listener)
				_handleListenerEvent( *listener );
			else if (_isCgiPipe(fd) == true)
				_handleCgiEvent( fd, events[i].events );
			else
				_handleClientEvent( fd, events[i].events );
		}
	}
	_killAllUpCgi();
}
