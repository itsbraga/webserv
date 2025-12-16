/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:35:06 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/16 03:28:48 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <vector>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>

# include "defines.hpp"

class Response;

// string.cpp
std::vector<std::string>	split( const std::string& str, char delimiter );
void						erase_whitespace( std::string& str, const std::string& set );

// err_msg.cpp
void						err_msg( const std::string& context, const std::string& reason );
void						err_msg_quoted( const std::string& context, const std::string& reason );

/**************************\
 *	Template functions
\**************************/

template< typename T >
std::string		toString( const T& value )
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

/**************************\
 *	Request class tools
\**************************/

// Request_utils.cpp
std::string		extractRequestLine( const std::string& serialized );
void			parseRequestLine( const std::string& request_line, std::string& method, std::string& URI, std::string& protocol_version );
void			validateMethod( const std::string& method );
void			validateURI( const std::string& URI );
void			validateProtocolVersion( const std::string& protocolVersion );

bool			isValidHeaderName( const std::string& name );
size_t			findHeaderBoundaries( const std::string& serialized, size_t& header_start, size_t& header_end );

/**************************\
 *	File utils
\**************************/

// file_utils.cpp
bool			fileExists( const std::string& path );
bool			isDirectory( const std::string& path );
bool			isReadable( const std::string& path );
std::string		readFileContent( const std::string& path );
std::string		generateAutoindex( const std::string& path, const std::string& URI );

/**************************\
 *	Exceptions catcher
\**************************/

Response*		handleHttpException( const std::exception& e );