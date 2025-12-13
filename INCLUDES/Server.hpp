/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/13 23:57:54 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <iostream>
# include <string>
# include <cstring>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/epoll.h>
# include <sys/types.h>
# include <errno.h>

# include "defines.hpp"
# include "colors.hpp"

/**************************\
 *	Class
\**************************/

class LocationTree;

class Server
{
	private:
		int				_socket;
		sockaddr_in		_addr;
		std::string 	_server_name;
		uint16_t 		_port;
		LocationTree*	_tree;

		Server( const Server& );
		Server&			operator=( const Server& );

		bool			_createSocket();
		bool			_configureSocket();
		bool			_bindAndListen();
		bool			_setNonBlocking( int fd );
		void			_closeSocket();
		
	public:
		Server( std::string server_name, uint16_t port );
		~Server();

		bool			init();
		int				acceptNewClient();

		int				getSocket() const			{ return (_socket); }
		sockaddr_in		getAddress() const			{ return (_addr); }
		std::string		getServerName() const		{ return (_server_name); }
		uint16_t		getPort() const				{ return (_port); }
		LocationTree*	getLocationTree() const		{ return (_tree); }
};