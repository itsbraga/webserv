/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 18:44:33 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/26 16:20:12 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

bool	Listener::createSocketFd()
{
	socket_fd = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (socket_fd == -1)
	{
		err_msg( "socket()", strerror( errno ) );
		return (false);
	}
	return (true);
}

bool	Listener::configureSocket()
{
	int opt = 1;

	if (setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) ) == -1)
	{
		err_msg( "setsockopt()", strerror( errno ) );
		return (false);
	}
	return (true);
}

bool	Listener::bindAndListen()
{
	std::memset( &addr, 0, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	addr.sin_port = htons( port );

	// Associates the socket with a specific IP address and port
	if (bind( socket_fd, reinterpret_cast<sockaddr*>( &addr ), sizeof( addr ) ) == -1)
	{
		err_msg( "bind()", strerror( errno ) );
		return (false);
	}

	// Marks the socket as a passive socket for incoming connection requests (telephone switchboard)
	if (listen( socket_fd, SOMAXCONN ) == -1)
	{
		err_msg( "listen()", strerror( errno ) );
		return (false);
	}
	return (true);
}

bool	Listener::setNonBlocking( int fd )
{
	if (fcntl( fd, F_SETFL, O_NONBLOCK ) == -1)
	{
		err_msg( "fcntl(F_SETFL | O_NONBLOCK)", strerror( errno ) );
		return (false);
	}
	return (true);
}

void	Listener::closeSocketFd()
{
	if (socket_fd != -1)
	{
		::close( socket_fd );
		socket_fd = -1;
	}
}

bool	Listener::init()
{
	if (!createSocketFd())
		return (false);
	if (!configureSocket() || !bindAndListen() || !setNonBlocking( socket_fd ))
	{
		closeSocketFd();
		return (false);
	}
	return (true);
}

int		Listener::acceptClient()
{
	int client_fd = ::accept( socket_fd, NULL, NULL );
	if (client_fd == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			err_msg( "accept()", strerror( errno ) );
		return (-1);
	}

	if (!setNonBlocking( client_fd ))
	{
		::close( client_fd );
		return (-1);
	}
	return (client_fd);
}

ServerConfig&	Listener::resolveVirtualHosting( const std::string& host )
{
	std::string hostname = host;

	size_t colon = host.find( ':' );
	if (colon != std::string::npos)
		hostname = host.substr( 0, colon );

	for (size_t i = 0; i < servers.size(); ++i)
	{
		if (servers[i]->getServerName() == hostname)
			return (*servers[i]);
	}
	return (*servers[0]);
}

void	Listener::printInfo() const
{
	std::cout << P_PURPLE "Listening on port " << port << " for: ";

	for (size_t i = 0; i < servers.size(); ++i)
	{
		if (i > 0)
			std::cout << ", ";
		std::cout << "[" << servers[i]->getServerName() << "]";
	}
	std::cout << NC "\n";
}
