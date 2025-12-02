/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/02 19:40:22 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

// C++
# include <iostream>
# include <fstream>
# include <sstream> 
# include <vector>
# include <map>
# include <set>
# include <string>
# include <cstring>
# include <cctype>
# include <cstdio>
# include <ctime>

// C
# include <netinet/in.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/epoll.h>
# include <sys/stat.h>

// Our libs
# include "Parser.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "LocationTree.hpp"
# include "Message.hpp"
# include "Request.hpp"
# include "Response.hpp"

# include "defines.hpp"
# include "colors.hpp"

typedef struct webserv_s
{
	unsigned int		servers_nb;
	std::vector<Server> servers;
}		webserv_t;

/**************************\
 *	Template functions
\**************************/

template< typename T >
std::string	toString( const T& value )
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

/**************************\
 *	UTILS
\**************************/

// string.cpp
std::vector<std::string>	split(std::string str, char delimiter);

// err_msg.cpp
void	err_msg( const std::string& context, const std::string& reason );
void	err_msg_quoted( const std::string& context, const std::string& reason );

/**************************\
 *	UTILS/INIT
\**************************/

// init_method_map.cpp
void	init_method_map();
