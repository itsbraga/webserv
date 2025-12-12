/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:35:06 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/12 16:34:00 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <sstream>
# include <string>
# include <vector>

# include "defines.hpp"

// string.cpp
std::vector<std::string>	split( std::string str, char delimiter );
void						erase_whitespace( std::string& str, std::string& set );

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

std::string		extractRequestLine( const std::string& serialized );
void			parseRequestLine( const std::string& request_line, std::string& method, std::string& URI, std::string& protocol_version );
void			validateMethod( const std::string& method );
void			validateURI( const std::string& URI );
void			validateProtocolVersion( const std::string& protocolVersion );

bool			isValidHeaderName( const std::string& name );
size_t			findHeaderBoundaries( const std::string& serialized, size_t& header_start, size_t& header_end );