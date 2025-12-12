/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:06:53 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/12 21:15:37 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Server::Server( uint16_t port, std::string server_name )
				: _port( port ), _server_name( server_name ), _tree( NULL ), _socket( -1 )
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
	------------------------- [ Private method ] -------------------------
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
	------------------------- [ Public methods ] -------------------------
*/
bool	Server::init()
{
	if (_createSocket() == false)
		return (false);
	if (_configureSocket() == false || _bindAndListen() == false || _setNonBlocking( _socket ) == false)
	{
		_closeSocket();
		return (false);
	}
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
	if (_setNonBlocking( clientSocket ) == false)
	{
		::close( clientSocket );
		return (-1);
	}
	return (clientSocket);
}