/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:35:06 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/26 23:44:52 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Basic utilities
\**************************/

// err_msg.cpp
void						err_msg( const char *context, const std::string& reason );
void						err_msg_quoted( const char *context, const std::string& reason );

// string.cpp
std::vector<std::string>	split( const std::string& str, char delimiter );
void						lowerStr( std::string& str );
std::string					toLower( const std::string& str );
std::string					extractQueryString( const std::string &uri );
std::string					decodeUri( const std::string& uri );

/**************************\
 *	Template function
\**************************/

template< typename T >
std::string		toString( const T& value )
{
	std::ostringstream oss;
	oss << value;

	return (oss.str());
}

/**************************\
 *	Request class utils
\**************************/

// request/Request_utils.cpp
std::string		extractRequestLine( const std::string& serialized );
void			parseRequestLine( const std::string& request_line, std::string& method, std::string& uri, std::string& protocol_version );
void			validateMethod( const std::string& method );
void			validateUri( const std::string& uri );
void			validateProtocolVersion( const std::string& protocolVersion );

bool			isValidHeaderName( const std::string& name );
size_t			findHeaderBoundaries( const std::string& serialized, size_t& header_start, size_t& header_end );

bool			isReturn(const Request& request, const ServerConfig& server);
Response*		returnHandler(const Request& request, const ServerConfig& server);

/**************************\
 *	POST method utils
\**************************/

// methods/handlePOST_utils.cpp
std::string		handleUpload( const std::string& body, const std::string& content_type, const std::string& upload_dir, const std::string& upload_route );
size_t			convertBodySize( const std::string& value );

/**************************\
 *	File
\**************************/

class Location;

// file.cpp
bool			pathExists( const std::string& path );
std::string		resolvePath( Location& route, const std::string& uri );

bool			isRegularFile( const std::string& path );
bool			isReadable( const std::string& path );
bool			isWritable( const std::string& path );
std::string		readFileContent( const std::string& path );
bool			saveFile( const std::string& path, const std::string& content );
bool			deleteFile( const std::string& path );
std::string		extractBasename( const std::string& filename );
bool			isValidFilename( const std::string& filename );

/**************************\
 *	Directory
\**************************/

// directory.cpp
bool			isDirectory( const std::string& path );
std::string		getParentDir( const std::string& path );
bool			isSafePath( const std::string& root, const std::string& requested_path );

/**************************\
 *	Autoindex
\**************************/

// autoindex.cpp
std::string		generateAutoIndex( const std::string& path, const std::string& uri );
