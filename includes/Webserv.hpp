/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/29 20:12:05 by annabrag         ###   ########.fr       */
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
# include <climits>
# include <vector>
# include <map>
# include <set>
# include <csignal>

// External C
# include <sys/epoll.h>
# include <sys/select.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <dirent.h>
# include <unistd.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <stdlib.h>
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
# include "Cgi.hpp"

void	signal_handler( int __attribute__((unused))signo );

/**************************\
 *	Method Handler
\**************************/

class ServerConfig;
class Request;
class Response;

typedef Response* (*MethodHandler)( const ServerConfig& server, const Request& request );

extern std::map<std::string, MethodHandler> g_method_map;

/**************************\
 *	Class
\**************************/

class Client;

class Webserv
{
	private:
		int							_epoll_fd;
		std::vector<ServerConfig>	_servers;
		std::vector<Listener>		_listeners;
		std::map<int, Client>		_clients;
		std::set<pid_t>				_up_cgis;

		Listener*	_getListenerByFd( int fd );

		bool		_addServerToEpoll( int server_fd );
		bool		_addClientToEpoll( int client_fd );
		void		_removeClient( int client_fd );
		bool		_modifyEpollEvents( int fd, unsigned int events );

		void		_handleListenerEvent( Listener& listener );
		void		_handleClientEvent( int client_fd, unsigned int events );
		void		_handleClientRead( int client_fd );
		void		_handleClientWrite( int client_fd );

		Response*	_executeRequest( Request& request, ServerConfig& server );
		void		_processRequest( int client_fd );

		void		_checkClientTimeout();
		void		_killAllUpCgi();

		Webserv( const Webserv& toCopy );
		Webserv&	operator=( const Webserv& toCopy );

	public:
		Webserv();
		~Webserv();

		void		addServerConfig( ServerConfig& server );
		bool		initListeners();
		bool		initEpoll();
		void		run();

		void		addCgiPid( pid_t pid ) 		{ _up_cgis.insert( pid ); }
		void		removeCgiPid( pid_t pid ) 	{ _up_cgis.erase( pid ); };
};

/**************************\
 *	Methods
\**************************/

// method_map.cpp
void			init_method_map();
Response*		methodHandler( const ServerConfig& server, const Request& request );
Response*		httpExceptionHandler( const std::exception& e );

// handleGET.cpp
Response*		handleGET( const ServerConfig& server, const Request& request );

// handlePOST.cpp
Response*		handlePOST( const ServerConfig& server, const Request& request );

// handleHEAD.cpp
Response*		handleHEAD( const ServerConfig& server, const Request& request );

// handleDELETE.cpp
Response*		handleDELETE( const ServerConfig& server, const Request& request );
