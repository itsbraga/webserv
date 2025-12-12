/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/12 16:19:01 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

// C++
# include <iostream>
# include <map>
# include <set>

// C
# include <sys/wait.h>		// waitpid()

// Project
# include "defines.hpp"
# include "colors.hpp"
# include "utils.hpp"
# include "Parser.hpp"
# include "Location.hpp"
# include "LocationTree.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Message.hpp"
# include "Request.hpp"
# include "Response.hpp"

/**************************\
 *	Class
\**************************/

class Client;

class Webserv
{
	private:
		int						_epoll_fd;
		std::map<int, Server*>	_servers;
		std::map<int, Client*>	_clients;
		
		bool	_addServerToEpoll( Server* server );
		bool	_addClientToEpoll( int client_fd );
		void	_removeClient( int client_fd );

		void	_handleServerEvent( int server_fd );
		void	_handleClientEvent( int client_fd, uint32_t events );
		void	_handleClientRead( int client_fd, Client* client );
		void	_processRequest( int client_fd, Client* client );

	public:
		Webserv() : _epoll_fd( -1 ) {}
		~Webserv();

		bool	addServer( uint16_t port, const std::string& server_name );
		bool	init(/* config file */);
		void	run();
};

/**************************\
 *	Functions
\**************************/

// init_method_map.cpp
void	init_method_map();