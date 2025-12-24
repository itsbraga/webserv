/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/24 17:46:13 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

// External C++
# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <ctime>
# include <cctype>
# include <cstring>
# include <cstdlib>
# include <utility>
# include <algorithm>
# include <exception>
# include <limits>
# include <vector>
# include <map>

// External C
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <dirent.h>
# include <unistd.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <errno.h>

// Project
# include "defines.hpp"
# include "colors.hpp"
# include "utilities.hpp"
# include "HttpExceptions.hpp"
# include "Token.hpp"
# include "Parser.hpp"
# include "ErrorPage.hpp"
# include "Location.hpp"
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
		std::map<int, Server>	_servers;
		std::map<int, Client>	_clients;

		bool	_addServerToEpoll( int server_fd );
		bool	_addClientToEpoll( int client_fd );
		void	_removeClient( int client_fd );

		void	_handleServerEvent( int server_fd );
		void	_handleClientEvent( int client_fd, unsigned int events );
		void	_handleClientData( int client_fd );
		void	_processRequest( int client_fd );
		void	_checkClientTimeout();

		Webserv( const Webserv& toCopy );
		Webserv&	operator=( const Webserv& toCopy );

	public:
		Webserv();
		~Webserv();

		bool	addServer( Server& server );

		Server*			getServer( int fd );
		const Server*	getServer( int fd ) const;

		bool	init();
		void	run();
};

/**************************\
 *	Methods
\**************************/

// method_map.cpp
void		init_method_map();
Response*	handleMethod( Server& server, Request& request );

// handleGET.cpp
Response*	handleGET( Server& server, Request& request );

// handlePOST.cpp
Response*	handlePOST( Server& server, Request& request );

// handleHEAD.cpp
Response*	handleHEAD( Server& server, Request& request );

// handleDELETE.cpp
Response*	handleDELETE( Server& server, Request& request );
