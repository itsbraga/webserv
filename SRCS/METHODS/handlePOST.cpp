/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePOST.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 00:00:23 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/16 02:34:07 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__resolvePath( Server& server, const std::string& URI )
{
	return (server.getRoot() + URI);
}

static std::string	__getParentDir( const std::string& path )
{
	size_t pos = path.find_last_of( '/' );
	if (pos == std::string::npos)
		return (".");
	return (path.substr( 0, pos ));
}

static bool		__isWritable( const std::string& path )
{
	return (access( path.c_str(), W_OK ) == 0);
}

static void		__saveFile( const std::string& path, const std::string& content )
{
	std::ofstream file( path.c_str(), std::ios::binary );
	if (!file.is_open())
		throw InternalServerErrorException();

	file.write( content.c_str(), content.size() );
}

Response*	handlePOST( Server& server, Request& request )
{
	std::string URI = request.getURI();
	std::string path = __resolvePath( server, URI );
	std::string body = request.getBody();
	std::string parentDir = __getParentDir( path );

	if (body.empty())
		throw BadRequestException();
	if (!fileExists( parentDir ) || !isDirectory( parentDir ))
		throw NotFoundException();
	if (!__isWritable( parentDir ))
		throw ForbiddenException();

	bool exists = fileExists( path );
	__saveFile( path, body );

	Response* response = new Response( exists ? 200 : 201, exists ? "OK" : "Created" );
	response->setBody( exists ? "File updated\n" : "File created\n" );
	response->setContentLength( "13" );
	response->setContentType( "text/plain" );
	if (!exists)
		response->setLocation( URI );
	return (response);
}