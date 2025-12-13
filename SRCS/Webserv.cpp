/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/14 00:02:01 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Webserv::~Webserv()
{
	if (_epoll_fd != -1)
		::close( _epoll_fd );

	std::map<int, Client*>::iterator it;

	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		::close( it->first );
		delete it->second;
	}

	std::map<int, Server*>::iterator it2;

	for (it2 = _servers.begin(); it2 != _servers.end(); ++it2)
		delete it2->second;
}

/*
	------------------------ [ Private methods ] -------------------------
*/
bool	Webserv::_addServerToEpoll( Server* server )
{
	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = server->getSocket();

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "epoll_ctl(ADD server): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

bool	Webserv::_addClientToEpoll( int client_fd )
{
	epoll_event ev;
	std::memset( &ev, 0, sizeof( ev ) );
	ev.events = EPOLLIN;
	ev.data.fd = client_fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, client_fd, &ev ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "epoll_ctl(ADD client): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

void	Webserv::_removeClient( int client_fd )
{
	epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, client_fd, NULL );
	::close( client_fd );
	delete _clients[client_fd];
	_clients.erase( client_fd );
}

void	Webserv::_handleServerEvent( int server_fd )
{
	Server* server = _servers[server_fd];

	while (true)
	{
		int client_fd = server->acceptNewClient();
		if (client_fd == -1)
			break ;		
		if (_addClientToEpoll( client_fd ) == false)
		{
			::close( client_fd );
			continue ;
		}
		_clients.insert( std::make_pair( client_fd, new Client( client_fd, server ) ) );
		std::cout << P_BLUE "[INFO] " NC "Client accepted (fd=" << client_fd << ")" << std::endl; 
	}
}

void	Webserv::_handleClientEvent( int client_fd, uint32_t events )
{
	if (_clients.find( client_fd ) == _clients.end() )
		return ;

	Client*	client = _clients[client_fd];

	if (events & EPOLLERR)
	{
		_removeClient( client_fd );
		std::cout << P_BLUE "[INFO] " NC "Client closed (EPOLLERR)" << std::endl;
		return ;
	}
	if (events & EPOLLIN)
		_handleClientRead( client_fd, client );
}

void	Webserv::_handleClientRead( int client_fd, Client* client )
{
	char buffer[8192];
	ssize_t nBytes = ::recv( client_fd, buffer, sizeof( buffer ), 0 );

	if (nBytes < 0)
	{
		std::cout << ERR_PREFIX << P_ORANGE "recv(): " NC << strerror( errno ) << std::endl;
		return ;
	}
	if (nBytes == 0)
	{
		_removeClient( client_fd );
		std::cout << P_BLUE "[INFO] " NC "Client disconnected (fd=" << client_fd << ")" << std::endl;
		return ;
	}

	// pour telnet
	// std::cout << P_ORANGE "[DEBUG] " NC "Received " << nBytes << " bytes from client_fd=" << client_fd << ": ";
	// std::cout.write(buffer, nBytes);

	client->appendToReadBuffer( buffer, nBytes );

	// std::cout << P_ORANGE "[DEBUG] " NC "Buffer size: " << client->getReadBuffer().size()
	// 		  << ", complete: " << (client->hasCompleteRequest() ? "YES" : "NO") << std::endl;

	if (client->hasCompleteRequest())
		_processRequest( client_fd, client );
}

void	Webserv::_processRequest( int client_fd, Client* client )
{
	try {
		Request request( client->getReadBuffer() );

		std::cout << P_YELLOW "\n--- Request received ---" NC << std::endl;
		// std::cout << client->getReadBuffer() << std::endl;
		std::cout << "Method: " << request.getMethod() << std::endl;
		std::cout << "URI: " << request.getURI() << std::endl;
		std::cout << "Headers:\n" << request.getHeaderMap() << std::endl;
		// std::cout << P_YELLOW "------------------------\n" NC << std::endl;

		Response response( 200, "OK" );

		std::string serialized = response.getSerializedResponse();
		::send( client_fd, serialized.c_str(), serialized.size(), 0 );
	}
	catch (const SyntaxErrorException& e) {
		std::cerr << e.what() << std::endl;
		// Response response( 400, "Bad Request" );
		// std::string serialized = response.getSerializedResponse();
		// ::send( client_fd, serialized.c_str(), serialized.size(), 0 );
	}

	client->clearReadBuffer();
}

/*
	------------------------- [ Public methods ] -------------------------
*/
bool	Webserv::addServer( const std::string& server_name, uint16_t port )
{
	Server* server = new Server( server_name, port );

	if (server->init() == false)
	{
		delete server;
		return (false);
	}
	_servers.insert( std::make_pair( server->getSocket(), server ) );
	return (true);
}

bool	Webserv::init(/* config file */)
{
	Response::initBuilders();
	Response::initContentTypes();

	_epoll_fd = epoll_create(1);
	if (_epoll_fd == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "epoll_create(): " NC << strerror( errno ) << std::endl;
		return (false);
	}

	std::map<int, Server*>::iterator it;

	for (it = _servers.begin(); it != _servers.end(); ++it)
	{
		if (_addServerToEpoll( it->second ) == false)
			return (false);
	}
	return (true);
}

void	Webserv::run()
{
	epoll_event events[MAX_EVENTS];

	std::cout << P_YELLOW "[Webserv] Starting event loop...\n" NC << std::endl;

	while (true)
	{
		int nbReady = epoll_wait( _epoll_fd, events, MAX_EVENTS, -1 );
		if (nbReady == -1)
		{
			if (errno == EINTR)
				continue ;
			std::cerr << ERR_PREFIX << P_ORANGE "epoll_wait(): " NC << strerror( errno ) << std::endl;
			break ;
		}

		for (int i = 0; i < nbReady; i++)
		{
			int fd = events[i].data.fd;
			if (_servers.count( fd ) > 0)
				_handleServerEvent( fd );
			else
				_handleClientEvent( fd, events[i].events );
		}
	}
}
