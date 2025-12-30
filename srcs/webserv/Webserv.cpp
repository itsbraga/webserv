/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/30 20:29:18 by pmateo           ###   ########.fr       */
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
Response*	Webserv::_executeRequest( int client_fd, Request& request, Listener& listener )
{
	__requestReceived( request );

	std::string hostname = request.getHeaderValue( "host" );
	ServerConfig server = listener.resolveVirtualHosting( hostname );

	if (isReturn( request, server ))
	{
		std::cerr << "RETURN\n";
		return (returnHandler( request, server ));
	}
	else if (isCgiRequest( request, server ))
	{
		std::cerr << "CGI\n";
		return (cgiHandler( client_fd, request, server, (*this) ));
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
		response = _executeRequest( client_fd, request, *listener );
		if (response == NULL) //WaitForCgi
			return ;
	}
	catch (const BadRequestException& e) {
		close_client = true;
		std::cerr << P_YELLOW "[DEBUG] " << e.what() << NC << std::endl;
		response = httpExceptionHandler( e );
	}
	catch (const ChildErrorException&e ) {
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
	-------------------- [ CGI ] ---------------------
*/
void 	Webserv::_handleCgiEvent(int pipe_fd, unsigned int events)
{

	std::cerr << "[DEBUG] _handleCgiEvent called, pipe_fd=" << pipe_fd 
              << " events=" << events << std::endl;
	std::map<int, int>::iterator it = _cgi_pipes.find(pipe_fd);
	if (it == _cgi_pipes.end())
	{
		std::cerr << "[DEBUG] pipe_fd not found in _cgi_pipes!" << std::endl;
		return ;
	}

	int client_fd = it->second;

	std::map<int, Client>::iterator client_it = _clients.find(client_fd);
	if (client_it == _clients.end())
	{
		epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, pipe_fd, NULL);
		close(pipe_fd);
		_cgi_pipes.erase(it);
		return ;
	}

	Client& client = client_it->second;

	if (events & (EPOLLERR | EPOLLHUP))
		_handleCgiResponse(client_fd);

	if (events & EPOLLIN)
	{
		char buffer[4096];
		ssize_t bytes = read(pipe_fd, buffer, sizeof(buffer));
		std::cerr << "[DEBUG] read() returned " << bytes << std::endl;
		if (bytes > 0)
		{
			client.getCgiOuput().append(buffer, bytes);
			client.setCgiLastRead(time(NULL));
		}
		else if (bytes == 0)
		{
			std::cerr << "[DEBUG] EOF detected, calling _handleCgiResponse" << std::endl;
			_handleCgiResponse(client_fd);
		}
		else
		{
			std::cerr << "[DEBUG] read error: " << strerror(errno) << std::endl;
			_killCgi(client_fd, 500);
		}
	}
}

void	Webserv::_handleCgiResponse(int client_fd)
{
	std::map<int, Client>::iterator it = _clients.find(client_fd);
	if (it == _clients.end())
		return;
	Client& client = it->second;

	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client.getCgiPipe(), NULL);
	close(client.getCgiPipe());
	_cgi_pipes.erase(client.getCgiPipe());

	int status;
	waitpid(client.getCgiPid(), &status, 0);
	removeCgiPid(client.getCgiPid());

	Response *response = NULL;
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == SUCCESS)
			response = handleOutput(client.getCgiOuput());
		else if (WEXITSTATUS(status) == CGI_ERROR)
			response = new Response(502, "Bad Gateway");
		else
			response = new Response(500, "Internal Server Error");
	}
	else if (WIFSIGNALED(status))
		response = new Response(502, "Bad Gateway");
	else
		response = new Response(502, "Bad Gateway");
	bool close = client.shouldClose();
	if (response->getHeaderValue("connection") == "close")
		close = true;
	client.setShouldClose(close);

	std::string serialized = response->getSerializedResponse();
	delete response;
	client.appendToWriteBuffer(serialized);
	client.setWaitForCgi(false);
	client.getCgiOuput().clear();
	_modifyEpollEvents(client_fd, EPOLLIN | EPOLLOUT);
}

bool 	Webserv::_isCgiPipe(int fd) const
{
	return (_cgi_pipes.find(fd) != _cgi_pipes.end());
}

void	Webserv::_killCgi(int client_fd, int status_code)
{
	std::map<int, Client>::iterator it = _clients.find(client_fd);
	if (it == _clients.end())
		return ;
	Client& client = it->second;
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client.getCgiPipe(), NULL);
	close(client.getCgiPipe());
	_cgi_pipes.erase(client.getCgiPipe());
	kill(client.getCgiPid(), SIGKILL);
	waitpid(client.getCgiPid(), NULL, 0);
	removeCgiPid(client.getCgiPid());
	Response* response = NULL;
	if (status_code == 504)
		response = new Response(504, "Gateway Timeout");
	else
		response = new Response(500, "Internal Server Error");
	bool close = client.shouldClose();
	if (response->getHeaderValue("connection") == "close")
		close = true;
	client.setShouldClose(close);
	std::string serialized = response->getSerializedResponse();
	delete response;
	client.appendToWriteBuffer(serialized);
	client.setWaitForCgi(false);
	client.getCgiOuput().clear();
	_modifyEpollEvents(client_fd, EPOLLIN | EPOLLOUT);
}

void	Webserv::_checkCgiTimeout()
{
	time_t tmp = time(NULL);

	std::map<int, Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
	{
		Client &client = it->second;
		if (client.getWaitForCgi() == false)
			continue;

		if (tmp - client.getCgiLastRead() > CGI_INACTIVITY_TIMEOUT)
		{
			_killCgi(it->first, 504);
			continue;
		}

		if (tmp - client.getCgiStart() > CGI_SLOWLORIS_TIMEOUT)
		{
			_killCgi(it->first, 504);
			continue;
		}
	}
}

void	Webserv::_killAllUpCgi()
{
	std::map<int, Client>::iterator client_it = _clients.begin();
	for(; client_it != _clients.end(); ++client_it)
	{
		Client& client = client_it->second;
		if (client.getWaitForCgi() == true)
		{
			epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client.getCgiPipe(), NULL);
			close(client.getCgiPipe());
			_cgi_pipes.erase(client.getCgiPipe());
			kill(client.getCgiPid(), SIGKILL);
			waitpid(client.getCgiPid(), NULL, 0);
			removeCgiPid(client.getCgiPid());
		}
	}

	std::set<pid_t>::const_iterator it = _up_cgis.begin();

	for (; it != _up_cgis.end(); ++it)
	{
		kill( *it, SIGKILL );
		waitpid( *it, NULL, 0 );
	}
	_up_cgis.clear();
}

void	Webserv::cleanUpForChild()
{
	std::map<int, Client>::iterator client_it = _clients.begin();

	for (; client_it != _clients.end(); ++client_it)
		::close( client_it->first );

	for (size_t i = 0; i < _listeners.size(); ++i)
		_listeners[i].closeSocketFd();

	if (_epoll_fd != -1)
		::close( _epoll_fd );

	std::map<int, int>::iterator it = _cgi_pipes.begin();
	for (; it != _cgi_pipes.end(); ++it)
		close(it->first);
	// _cgi_pipes.clear();
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
	std::cout << BOLD P_YELLOW "===== initListeners =====\n" NC << std::endl;
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
		int nbFds = epoll_wait( _epoll_fd, events, MAX_EVENTS, 1000 );
		if (nbFds == -1)
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

		for (int i = 0; i < nbFds; ++i)
		{
			if (g_stop == 1)
				break;

			int fd = events[i].data.fd;
			Listener* listener = _getListenerByFd( fd );
			if (listener)
			{
				std::cerr << "[DEBUG] -> Listener event" << std::endl;
				_handleListenerEvent( *listener );
			}
			else if (_isCgiPipe(fd) == true)
			{
				std::cerr << "[DEBUG] -> CGI pipe event" << std::endl;
				_handleCgiEvent( fd, events[i].events );
			}
			else
			{
				std::cerr << "[DEBUG] -> Client event" << std::endl;
				_handleClientEvent( fd, events[i].events );
			}
		}
	}
	_killAllUpCgi();
}
