/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/03 19:27:44 by annabrag         ###   ########.fr       */
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
		std::vector<Server*>	_servers;
		std::map<int, Client>	_clients;
		std::set<int>			_serverFds;

		void	_handleServerEvent( int serverFd );
		void	_handleClientEvent( int clientFd, uint32_t events );

	public:
		Webserv() : _epollFd(-1) {}
		~Webserv();

		int		addServer( uint16_t port, const std::string& server_name );

		int		init(/* config */);
		// void	run(); // main loop
};