/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:06:53 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/24 15:41:07 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object manipulation ] -----------------------
*/
Server::Server() : _socket( -1 ), _port( 0 ), _auto_index( false ), _return_code( 0 )
{
	std::memset( &_addr, 0, sizeof(_addr) );
}

Server::~Server()
{
	if (_socket != -1)
		::close( _socket );
}

Server&		Server::operator=( const Server& toCopy )
{
	if (this != &toCopy)
	{
		if (_socket != -1)
			::close( _socket );
		_socket = toCopy._socket;
		_addr = toCopy._addr;
		_port = toCopy._port;
		_server_name = toCopy._server_name;
		_root = toCopy._root;
		_index = toCopy._index;
		_auto_index = toCopy._auto_index;
		_client_max_size_body = toCopy._client_max_size_body;
		_return_code = toCopy._return_code;
		_return_uri = toCopy._return_uri; 
		_err_page = toCopy._err_page;
		_locations = toCopy._locations;

		const_cast<Server&>( toCopy )._socket = -1;
	}
	return (*this);
}

/*
	------------------------ [ Private methods ] -------------------------
*/
bool	Server::_createSocket()
{
	_socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (_socket == -1)
	{
		err_msg( "socket()", strerror( errno ) );
		return (false);
	}

	return (true);
}

bool	Server::_configureSocket()
{
	int opt = 1;

	if (setsockopt( _socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) ) == -1)
	{
		err_msg( "setsockopt()", strerror( errno ) );
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
		err_msg( "bind()", strerror( errno ) );
		return (false);
	}
	// Marks the socket as a passive socket for incoming connection requests (telephone switchboard)
	if (listen( _socket, SOMAXCONN ) == -1)
	{
		err_msg( "listen()", strerror( errno ) );
		return (false);
	}

	return (true);
}

bool	Server::_setNonBlocking( int fd )
{
	if (fcntl( fd, F_SETFL, O_NONBLOCK ) == -1)
	{
		err_msg( "fcntl(F_SETFL | O_NONBLOCK)", strerror( errno ) );
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
void	Server::setPort( unsigned short int port )
{
	_port = port;
}

void	Server::setServerName( const std::string& server_name )
{
	if (server_name.empty())
		return ;

	_server_name = server_name;
}

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

void	Server::setClientMaxSizeBody( const std::string& max_size )
{
	if (max_size.empty())
		return ;

	_client_max_size_body = max_size;
}

void	Server::setAutoIndex( bool auto_index )
{
	_auto_index = auto_index;
}

void	Server::setReturnCode( unsigned int return_code )
{
	_return_code = return_code;
}

void	Server::setReturnUri( const std::string& return_uri )
{
	if (return_uri.empty())
		return ;

	_return_uri = return_uri;
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
	std::cout << BOLD P_PURPLE "[" << _server_name << "] " NC P_PURPLE "Listening on port " << _port << NC "\n\n";

	return (true);
}

int		Server::acceptNewClient()
{
	int client_fd = ::accept( _socket, NULL, NULL );
	if (client_fd == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			err_msg( "accept()", strerror( errno ) );
		return (-1);
	}
	if (!_setNonBlocking( client_fd ))
	{
		::close( client_fd );
		return (-1);
	}

	return (client_fd);
}

// Location&	Server::findMatchingLocation( Server& server, const std::string& uri )
// {
	
// }


/*
	---------------------------- [ Operator ] ----------------------------
*/
std::ostream&	operator<<( std::ostream &os, const Server& server )
{
	os << RED << "###### [SERVER] ######" << NC << std::endl;

	os << P_BLUE << "PORT -> " << P_YELLOW << server.getPort() << NC << std::endl;
	if (server.getServerName() != "")
		os << P_BLUE << "SERVER_NAME -> " << P_YELLOW << server.getServerName() << NC << std::endl;
	if (server.getRoot() != "")
		os << P_BLUE << "ROOT -> " << P_YELLOW << server.getRoot() << NC << std::endl;
	if (server.getIndex() != "")
		os << P_BLUE << "INDEX -> " << P_YELLOW << server.getIndex() << NC << std::endl;
	if (server.getErrorPage().size() != 0)
	{
		for (size_t i = 0; i < server.getErrorPage().size(); ++i)
		{
			os << P_BLUE << "ERR_PAGE -> " << NC;
			for (size_t j = 0; j < server.getErrorPage()[i].getStatus().size(); ++j)
				os << P_YELLOW << server.getErrorPage()[i].getStatus()[j] << ' -> ';
			os << server.getErrorPage()[i].getFile() << NC << std::endl;
		}
	}
	os << P_BLUE << "AUTOINDEX -> " << P_YELLOW;
	if (server.getAutoIndex() == true)
		os << "true" << NC << std::endl;
	else
		os << "false" << NC << std::endl;
	if (server.getClientMaxSizeBody() != "")
		os << P_BLUE << "CLIENT_MAX_SIZE_BODY -> " << P_YELLOW << server.getClientMaxSizeBody() << NC << std::endl;
	if (server.getReturnCode() != 0)
		os << P_BLUE << "RETURN_CODE -> " << P_YELLOW << server.getReturnCode() << NC << std::endl;
	if (server.getReturnUri() != "")
		os << P_BLUE << "RETURN_URI -> " << P_YELLOW << server.getReturnUri() << NC << std::endl;
	os << P_BLUE << "LOCATION_MAP_SIZE -> " << P_YELLOW << server.getLocations().size() << NC << std::endl;

	return (os);
}