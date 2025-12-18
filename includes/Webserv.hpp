/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/18 21:28:35 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

// External
# include <iostream>
# include <map>
# include <set>
# include <sys/wait.h>		// waitpid()

// Project
# include "defines.hpp"
# include "colors.hpp"
# include "utilities.hpp"
# include "HttpExceptions.hpp"
# include "Parser.hpp"
# include "ErrorPage.hpp"
# include "Location.hpp"
# include "LocationTree.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Message.hpp"
# include "Request.hpp"
# include "Response.hpp"

/**************************\
 *	Method Handler
\**************************/

class Server;
class Request;
class Response;

typedef Response* (*MethodHandler)( Server& server, Request& request );

extern std::map<std::string, MethodHandler> g_method_map;

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
		void	_handleClientData( int client_fd, Client* client );
		void	_processRequest( int client_fd, Client* client );
		void	_checkClientTimeout();

	public:
		Webserv() : _epoll_fd( -1 ) {}
		~Webserv();

		bool	addServer( const std::string& server_name, uint16_t port );
		bool	init(/* config file */);
		void	run();
};

/**************************\
 *	METHODS
\**************************/

// method_map.cpp
void		init_method_map();
Response*	handleMethod( Server& server, Request& request );

// handleGET.cpp
Response*	handleGET( Server& server, Request& request );

// handlePOST.cpp
Response*	handlePOST( Server& server, Request& request );