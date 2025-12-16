/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:06:53 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/16 02:25:41 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
	---------------------- [ Object manipulation ] -----------------------
*/
Server::Server( const std::string& server_name, uint16_t port )
				: _socket( -1 ), _server_name( server_name ), _port( port ), _tree( NULL )
{
	if (server_name.empty())
		return ;

	std::memset( &_addr, 0, sizeof(_addr) );	
}

Server::~Server()
{
	if (_socket != -1)
		::close( _socket );
}

/*
	------------------------ [ Private methods ] -------------------------
*/
bool	Server::_createSocket()
{
	_socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (_socket == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "socket(): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

bool	Server::_configureSocket()
{
	int opt = 1;
	if (setsockopt( _socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "setsockopt(): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

bool	Server::_bindAndListen()
{
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons( _port );
	_addr.sin_addr.s_addr = htonl( INADDR_ANY );

	// Associates the socket with a specific IP address and port
	if (bind( _socket, reinterpret_cast<sockaddr*>( &_addr ), sizeof( _addr ) ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "bind(): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	// Marks the socket as a passive socket for incoming connection requests (telephone switchboard)
	if (listen( _socket, SOMAXCONN ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "listen(): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

bool	Server::_setNonBlocking( int fd )
{
	if (fcntl( fd, F_SETFL, O_NONBLOCK ) == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "fcntl(F_SETFL | O_NONBLOCK): " NC << strerror( errno ) << std::endl;
		return (false);
	}
	return (true);
}

void	Server::_closeSocket()
{
	if (_socket != -1)
	{
		::close( _socket );
		_socket = -1;
	}
}

/*
	---------------------------- [ Setters ] -----------------------------
*/
void	Server::setRoot( const std::string& root )
{
	if (root.empty())
		return ;

	_root = root;
}

void	Server::setIndex( const std::string& index )
{
	if (index.empty())
		return ;

	_index = index;
}

/*
	------------------------- [ Public methods ] -------------------------
*/
bool	Server::init()
{
	if (!_createSocket())
		return (false);
	if (!_configureSocket() || !_bindAndListen() || !_setNonBlocking( _socket ))
	{
		_closeSocket();
		return (false);
	}
	// en dur a supprimer apres
	setRoot( "./WWW" );
	setIndex( "index.html" );

	std::cout << BOLD P_PURPLE "[https://" << _server_name << ".xy] " NC P_BLUE "Listening on port " << _port
			  << NC "\n\n";
	return (true);
}

int		Server::acceptNewClient()
{
	int clientSocket = ::accept( _socket, NULL, NULL );
	if (clientSocket == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			std::cerr << ERR_PREFIX << P_ORANGE "accept(): " NC << strerror( errno ) << std::endl;
		return (-1);
	}
	if (!_setNonBlocking( clientSocket ))
	{
		::close( clientSocket );
		return (-1);
	}
	return (clientSocket);
}