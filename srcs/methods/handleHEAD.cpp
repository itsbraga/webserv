/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleHEAD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:38:47 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/25 18:37:56 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static Response*	__handleDirectoryRequest( const Location& route, std::string& path, const std::string& uri )
{
	std::string idx = path + (path[path.size() - 1] == '/' ? "" : "/") + route.getIndex();
	std::string	body;

	if (pathExists( idx ) && isRegularFile( idx ) && isReadable( idx ))
	{
		body = readFileContent( idx );
		path = idx;
	}
	else if (route.getAutoIndex())
		body = generateAutoIndex( path, uri );
	else
		throw ForbiddenException();

	Response* response = new Response( 200, "OK" );
	response->loadHeaders( body, path );

	return (response);
}

static Response*	__handleFileRequest( const std::string& path )
{
	std::string body = readFileContent( path );

	Response* response = new Response( 200, "OK" );
	response->loadHeaders( body, path );

	return (response);
}

Response*	handleHEAD( const ServerConfig& server, const Request& request )
{
	Location route = server.resolveRoute( request );

	if (!server.isMethodAllowed( route, "HEAD" ))
		throw MethodNotAllowedException();

	std::string	uri = request.getUri();
	std::string	path = resolvePath( route, uri );

	if (!pathExists( path ))
		throw NotFoundException();
	if (!isReadable( path ))
		throw ForbiddenException();
	if (isDirectory( path ))
		return (__handleDirectoryRequest( route, path, uri ));

	return (__handleFileRequest( path ));
}
