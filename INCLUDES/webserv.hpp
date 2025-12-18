/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/18 20:22:26 by pmateo           ###   ########.fr       */
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
# include "Message.hpp"
# include "Request.hpp"
# include "Response.hpp"

// Other
# include "defines.hpp"
# include "colors.hpp"

typedef struct webserv_s
{
	unsigned int		servers_nb;
	std::vector<Server> servers;
}		webserv_t;

/**************************\
 *	PARSING
\**************************/


/**************************\
 *	UTILS
\**************************/

// initMethodMap.cpp
void	init_method_map();

//string.cpp
std::vector<std::string>	split(std::string str, char delimiter);

// err_msg.cpp
void	err_msg( const std::string& context, const std::string& reason );
void	err_msg_quoted( const std::string& context, const std::string& reason );

template< typename T >
std::string	toString( const T& value )
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}
