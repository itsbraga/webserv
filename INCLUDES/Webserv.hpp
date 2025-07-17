/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/07/17 21:15:52 by pmateo           ###   ########.fr       */
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
# include <string>
# include <cstring>
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

// Our libs
# include "Message.hpp"
# include "Response.hpp"

// Other
# include "defines.hpp"
# include "colors.hpp"

/**************************\
 *	Utils
\**************************/

// initMethodMap.cpp
void	initMethodMap();

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
