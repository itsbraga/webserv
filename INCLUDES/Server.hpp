/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/03 18:21:39 by annabrag         ###   ########.fr       */
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
		uint16_t 		_port;
		std::string 	_server_name;
		LocationTree*	_tree;
		int				_socket;
		sockaddr_in		_addr;

		int			_setNonBlocking( int fd );
		
	public:
		Server( uint16_t port, std::string server_name );
		~Server();

		int			init();
		int			acceptNewClient();

		int			getSocket() const		{ return (_socket); }
		uint16_t	getPort() const			{ return (_port); }
		std::string	getServerName() const	{ return (_server_name); }
};