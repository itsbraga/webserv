/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/05 03:13:27 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Webserv::~Webserv()
{
	if (_epollFd != -1)
		::close( _epollFd );

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

	if (epoll_ctl( _epollFd, EPOLL_CTL_ADD, ev.data.fd, &ev ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "epoll_ctl(ADD server): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

bool	Webserv::_addClientToEpoll( int clientFd )
{
	epoll_event ev;
	std::memset( &ev, 0, sizeof( ev ) );
	ev.events = EPOLLIN;
	ev.data.fd = clientFd;

	if (epoll_ctl( _epollFd, EPOLL_CTL_ADD, clientFd, &ev ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "epoll_ctl(ADD client): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

void	Webserv::_removeClient( int clientFd )
{
	epoll_ctl( _epollFd, EPOLL_CTL_DEL, clientFd, NULL );
	::close( clientFd );
	delete _clients[clientFd];
	_clients.erase( clientFd );
}

void	Webserv::_handleServerEvent( int serverFd )
{
	Server* server = _servers[serverFd];

	while (true)
	{
		int clientFd = server->acceptNewClient();
		if (clientFd == -1)
			break ;		
		if (_addClientToEpoll( clientFd ) == false)
		{
			::close( clientFd );
			continue ;
		}
		_clients.insert( std::make_pair( clientFd, new Client( clientFd, server ) ) );
		std::cout << P_BLUE "[INFO] " NC "Client accepted (fd=" << clientFd << ")" << std::endl; 
	}
}

void	Webserv::_handleClientEvent( int clientFd, uint32_t events )
{
	if (_clients.find( clientFd ) == _clients.end() )
		return ;

	Client*	client = _clients[clientFd];

	if (events & EPOLLERR)
	{
		_removeClient( clientFd );
		std::cout << P_BLUE "[INFO] " NC "Client closed (EPOLLERR)" << std::endl;
		return ;
	}
	if (events & EPOLLIN)
		_handleClientRead( clientFd, client );
}

void	Webserv::_handleClientRead( int clientFd, Client* client )
{
	char buffer[8192];
	ssize_t nBytes = ::recv( clientFd, buffer, sizeof( buffer ), 0 );

	if (nBytes < 0)
	{
		std::cout << ERR_PREFIX << P_ORANGE "recv(): " NC << strerror( errno ) << std::endl;
		return ;
	}
	if (nBytes == 0)
	{
		_removeClient( clientFd );
		std::cout << P_BLUE "[INFO] " NC "Client disconnected (fd=" << clientFd << ")" << std::endl;
		return ;
	}

	// pour telnet
	std::cout << P_ORANGE "[DEBUG] " NC "Received " << nBytes << " bytes from clientFd=" << clientFd << ": ";
	std::cout.write(buffer, nBytes);

	client->appendToReadBuffer( buffer, nBytes );

	std::cout << P_ORANGE "[DEBUG] " NC "Buffer size: " << client->getReadBuffer().size()
			  << ", complete: " << (client->hasCompleteRequest() ? "YES" : "NO") << std::endl;

	if (client->hasCompleteRequest())
		_processRequest( clientFd, client );
}

void	Webserv::_processRequest( int clientFd, Client* client )
{
	std::cout << P_YELLOW "\n--- Request received ---" NC << std::endl;
	std::cout << client->getReadBuffer() << std::endl;
	std::cout << P_YELLOW "------------------------\n" NC << std::endl;

	// test
	const char	*response = "Received your request\n";
	::send( clientFd, response, strlen( response ), 0 );
	client->clearReadBuffer();
}

/*
	------------------------- [ Public methods ] -------------------------
*/
bool	Webserv::addServer( uint16_t port, const std::string& server_name )
{
	Server* server = new Server( port, server_name );

	if (server->init() == false)
	{
		delete server;
		return (false);
	}
	_servers.insert( std::make_pair( server->getSocket(), server ) );
	return (true);
}

bool	Webserv::init()
{
	_epollFd = epoll_create(1);
	if (_epollFd == -1)
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
		int nbReady = epoll_wait( _epollFd, events, MAX_EVENTS, -1 );
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
