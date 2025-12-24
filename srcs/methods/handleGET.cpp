/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:31:31 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/24 18:47:26 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static Response*	__handleDirectoryRequest( Server& server, std::string& path, const std::string& uri )
{
	std::string idx = path + (path[path.size() - 1] == '/' ? "" : "/") + server.getIndex();
	std::string	body;

	if (pathExists( idx ) && isRegularFile( idx ) && isReadable( idx ))
	{
		body = readFileContent( idx );
		path = idx;
	}
	else if (server.getAutoIndex())
		body = generateAutoIndex( path, uri );
	else
		throw ForbiddenException();

	Response* response = new Response( 200, "OK" );
	response->loadContent( body, path );

	return (response);
}

static Response*	__handleFileRequest( const std::string& path )
{
	std::string body = readFileContent( path );

	Response* response = new Response( 200, "OK" );
	response->loadContent( body, path );

	return (response);
}

Response*	handleGET( Server& server, Request& request )
{
	Location route = server.resolveRoute( request );

	if (!server.isMethodAllowed( route, "GET" ))
		throw MethodNotAllowedException();

	std::string	uri = request.getUri();
	std::string	path = resolvePath( route, uri );

	if (!pathExists( path ))
		throw NotFoundException();
	if (!isReadable( path ))
		throw ForbiddenException();
	if (isDirectory( path ))
		return (__handleDirectoryRequest( server, path, uri ));

	return (__handleFileRequest( path ));
}
