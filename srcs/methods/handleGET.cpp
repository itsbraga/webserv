/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:31:31 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/17 15:44:53 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__resolvePath( Server& server, const std::string& URI )
{
	if (URI == "/")
		return (server.getRoot() + "/" + server.getIndex());

	return (server.getRoot() + URI);
}

static Response*	__handleDirectoryRequest( Server& server, std::string& path, const std::string& URI )
{
	std::string idx = path + (path[path.size() - 1] == '/' ? "" : "/") + server.getIndex();
	std::string	body;

	if (fileExists( idx ) && !isDirectory( idx ))
	{
		body = readFileContent( idx );
		path = idx;
	}
	else
		body = generateAutoindex( path, URI );

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
	std::string	URI = request.getURI();
	std::string	path = __resolvePath( server, URI );

	if (!fileExists( path ))
		throw NotFoundException();
	if (!isReadable( path ))
		throw ForbiddenException();

	if (isDirectory( path ))
		return (__handleDirectoryRequest( server, path, URI ));
	return (__handleFileRequest( path ));
}
