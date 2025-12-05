/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/05 03:19:06 by art3mis          ###   ########.fr       */
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

// typedef struct webserv_s
// {
// 	unsigned int		servers_nb;
// 	std::vector<Server> servers;
// }		webserv_t;

/**************************\
 *	Class
\**************************/

class Client;

class Webserv
{
	private:
		int						_epollFd;
		std::map<int, Server*>	_servers;
		std::map<int, Client*>	_clients;
		
		bool	_addServerToEpoll( Server* server );
		bool	_addClientToEpoll( int clientFd );
		void	_removeClient( int clientFd );

		void	_handleServerEvent( int serverFd );
		void	_handleClientEvent( int clientFd, uint32_t events );
		void	_handleClientRead( int clientFd, Client* client );
		void	_processRequest( int clientFd, Client* client );

	public:
		Webserv() : _epollFd( -1 ) {}
		~Webserv();

		bool	addServer( uint16_t port, const std::string& server_name );
		bool	init(/* config file */);
		void	run();
};