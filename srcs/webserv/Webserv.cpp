/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/30 16:51:29 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Signal Handler ] -----------------------
*/
volatile sig_atomic_t g_stop = 0;

void	signal_handler(int __attribute__((unused))signo )
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
	if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
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
	------------------------ [ P: Epoll events ] -------------------------
*/
bool	Webserv::_addServerToEpoll( int server_fd )
{
	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = server_fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev ) == -1)
		return (err_msg( "epoll_ctl(ADD server)", strerror( errno ) ), false);
	else
		return (true);
}

bool	Webserv::_addClientToEpoll( int client_fd )
{
	epoll_event ev;
	std::memset( &ev, 0, sizeof( ev ) );
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = client_fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, client_fd, &ev ) == -1)
		return (err_msg( "epoll_ctl(ADD client)", strerror( errno ) ), false);
	else
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
		return (err_msg( "epoll_ctl(MODIFY)", strerror( errno ) ), false);
	else
		return (true);
}

/*
	--------------- [ P: Client & Listener events handling ] ---------------
*/
void	Webserv::_handleListenerEvent( Listener& listener )
{
	while (true)
	{
		int client_fd = listener.acceptClient();
		if (client_fd == -1)
			break;

		if (!_addClientToEpoll( client_fd ))
		{
			::close( client_fd );
			continue;
		}

		Client new_client( client_fd, listener.socket_fd );
		_clients.insert( std::make_pair( client_fd, new_client ) );
		std::cout << P_BLUE "[INFO] " NC "Client accepted (fd=" << client_fd << ")" << std::endl; 
	}
}

void	Webserv::_handleClientRead( int client_fd )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	Client& client = it->second;
	char buffer[8192];

	ssize_t nBytes = ::recv( client_fd, buffer, sizeof(buffer), 0 );
	if (nBytes > 0)
		client.appendToReadBuffer( buffer, nBytes );
	else if (nBytes == 0)
		return (_removeClient( client_fd ));

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
		if (client.shouldClose())
		{
			std::cout << P_BLUE "[INFO] " NC "Closing connection as requested..." << std::endl;
			return (_removeClient( client_fd ));
		}
		if (!_modifyEpollEvents( client_fd, EPOLLIN ))
			return ;
		client.clearReadBuffer();
		client.setRequestStartTime();
	}
}

void	Webserv::_handleClientEvent( int client_fd, unsigned int events )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	if (events & (EPOLLERR | EPOLLHUP))
		return (_removeClient( client_fd ));
	if (events & EPOLLIN)
		_handleClientRead( client_fd );
	if (events & EPOLLOUT)
		_handleClientWrite( client_fd );
}

/*
	---------------------- [ P: Request handling ] ----------------------
*/
Response*	Webserv::_executeRequest( Request& request, ServerConfig& server )
{
	__requestReceived( request );

	if (isReturn( request, server ))
	{
		std::cerr << "RETURN\n";
		return (returnHandler( request, server ));
	}
	else if (isCgiRequest( request, server ))
	{
		std::cerr << "CGI\n";
		return (cgiHandler( request, server, (*this) ));
	}
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

		response = _executeRequest( request, server );
		ErrorPageHandler( *response, request, server );
	}
	catch (const BadRequestException& e) {
		close_client = true;
		std::cerr << P_YELLOW "[DEBUG] " << e.what() << NC << std::endl;
		response = httpExceptionHandler( e );
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
	_modifyEpollEvents( client_fd, EPOLLIN | EPOLLOUT );
}

/*
	------------------- [ P: Client timeout cleanup ] --------------------
*/
void	Webserv::_checkClientTimeout()
{
	std::map<int, Client>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		int client_fd = it->first;
		if (it->second.isTimedOut( CLIENT_INACTIVITY_TIMEOUT ))
		{
			++it;
			std::cout << P_BLUE "[INFO] " NC "Client inactive timeout (fd=" << client_fd << ")" << std::endl;
			_removeClient( client_fd );
			continue;
		}
		else if (it->second.isRequestTimedOut( CLIENT_SLOWLORIS_TIMEOUT ))
		{
			++it;
			std::cout << P_BLUE "[INFO] " NC "Client request timeout (fd=" << client_fd << ")" << std::endl;
			_removeClient( client_fd );
			continue;
		}
		else
			++it;
	}
}

/*
	-------------------- [ P: CGI process clean_up ] ---------------------
*/
void	Webserv::_killAllUpCgi()
{
	if (_up_cgis.empty())
		return ;

	std::set<pid_t>::const_iterator it = _up_cgis.begin();

	for (; it != _up_cgis.end(); ++it)
	{
		kill( *it, SIGKILL );
		waitpid( *it, NULL, 0 );
	}
	_up_cgis.clear();
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
		int nbReady = epoll_wait( _epoll_fd, events, MAX_EVENTS, -1 );
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

		for (int i = 0; i < nbReady; ++i)
		{
			if (g_stop == 1)
				break;

			int fd = events[i].data.fd;
			Listener* listener = _getListenerByFd( fd );
			if (listener)
				_handleListenerEvent( *listener );
			else
				_handleClientEvent( fd, events[i].events );
		}
		_checkClientTimeout();
	}
	_killAllUpCgi();
}
