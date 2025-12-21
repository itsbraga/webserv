/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/20 23:31:25 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
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

class Server
{
	private:
		int				_socket;
		sockaddr_in		_addr;
		std::string 	_server_name;
		std::string		_ip_host;
		uint16_t 		_port;
		std::string		_root;
		std::string		_index;

		Server( const Server& );
		Server&			operator=( const Server& );

		bool			_createSocket();
		bool			_configureSocket();
		bool			_bindAndListen();
		bool			_setNonBlocking( int fd );
		void			_closeSocket();
		
	public:
		Server();
		Server( const std::string& server_name, uint16_t port );
		~Server();

		bool			init();
		int				acceptNewClient();

		void			setRoot( const std::string& root );
		void			setIndex( const std::string& index );

		int						getSocket() const			{ return (_socket); }
		const sockaddr_in&		getAddress() const			{ return (_addr); }
		const std::string&		getServerName() const		{ return (_server_name); }
		uint16_t				getPort() const				{ return (_port); }
		const std::string&		getRoot() const				{ return (_root); }
		const std::string&		getIndex() const			{ return (_index); }
};