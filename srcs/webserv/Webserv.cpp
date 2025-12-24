/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/24 19:47:23 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object manipulation ] -----------------------
*/
Webserv::Webserv() : _epoll_fd( -1 ) {}

Webserv::~Webserv()
{
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
	------------------------ [ Private methods ] -------------------------
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
	ev.events = EPOLLIN;
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
	_clients.erase( client_fd );
}

// ajouter verif MAX_CONNECTIONS
void	Webserv::_handleServerEvent( int server_fd )
{
	std::map<int, Server>::iterator it = _servers.find( server_fd );
	if (it == _servers.end())
		return ;

	Server& server = it->second;

	while (true)
	{
		int client_fd = server.acceptNewClient();
		if (client_fd == -1)
			break ;

		if (!_addClientToEpoll( client_fd ))
		{
			::close( client_fd );
			continue ;
		}

		Client new_client( client_fd, server_fd );
		_clients.insert( std::make_pair( client_fd, new_client ) );

		std::cout << P_BLUE "[INFO] " NC "Client accepted (fd=" << client_fd << ")" << std::endl; 
	}
}

// EPOLLET a implementer
void	Webserv::_handleClientEvent( int client_fd, unsigned int events )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	if (events & EPOLLERR)
	{
		_removeClient( client_fd );
		std::cout << P_BLUE "[INFO] " NC "Client closed (EPOLLERR)" << std::endl;
		return ;
	}
	if (events & EPOLLIN)
		_handleClientData( client_fd );
}

void	Webserv::_handleClientData( int client_fd )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	Client& client = it->second;
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

	client.appendToReadBuffer( buffer, nBytes );
	client.updateLastActivity();

	if (client.hasCompleteRequest())
		_processRequest( client_fd );
}

void	Webserv::_processRequest( int client_fd )
{
	std::map<int, Client>::iterator cit = _clients.find( client_fd );
	if (cit == _clients.end())
		return ;

	Client& client = cit->second;

	std::map<int, Server>::iterator sit = _servers.find( client.getServerFd() );
	if (sit == _servers.end())
	{
		err_msg( NULL, "Server not found for client" );
		_removeClient( client_fd );
		return ;
	}

	Server& server = sit->second;
	Response* response = NULL;

	try {
		Request request( client.getReadBuffer() );
		__requestReceived( request );

		response = handleMethod( server, request );
	}
	catch (const BadRequestException& e) {
		std::cerr << P_YELLOW "[DEBUG] " NC << e.what() << std::endl;
		response = handleHttpException( e );
	}
	catch (const std::exception& e) {
		response = handleHttpException( e );
	}

	std::string serialized = response->getSerializedResponse();
	::send( client_fd, serialized.c_str(), serialized.size(), 0 );
	delete response;
	client.clearReadBuffer();
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
	----------------------------- [ Getters ] ----------------------------
*/
Server*		Webserv::getServer( int fd )
{
	std::map<int, Server>::iterator it = _servers.find( fd );
	if (it == _servers.end())
		return (NULL);

	return (&it->second);
}

const Server*	Webserv::getServer( int fd ) const
{
	std::map<int, Server>::const_iterator it = _servers.find( fd );
	if (it == _servers.end())
		return (NULL);

	return (&it->second);
}

/*
	------------------------- [ Public methods ] -------------------------
*/
bool	Webserv::addServer( Server& server )
{
	int fd = server.getSocket();
	if (fd == -1)
	{
		err_msg( NULL, "Cannot add server with invalid socket" );
		return (false);
	}

	server.setTmp( false );
	_servers.insert( std::make_pair( fd, server ) );

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

	std::map<int, Server>::iterator it = _servers.begin();

	for (; it != _servers.end(); ++it)
	{
		if (!_addServerToEpoll( it->first ))
			return (false);
	}
	
	return (true);
}

// handle signals & cleanup
void	Webserv::run()
{
	epoll_event events[MAX_EVENTS];

	std::cout << P_YELLOW "[Webserv] Waiting for new connections...\n" NC << std::endl;

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
		for (int i = 0; i < nbFds; i++)
		{
			int fd = events[i].data.fd;
			if (_servers.count( fd ) > 0)
				_handleServerEvent( fd );
			else
				_handleClientEvent( fd, events[i].events );
		}
	}
}
