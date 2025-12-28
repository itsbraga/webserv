/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleHEAD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:38:47 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/28 21:33:44 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static Response*	__handleDirectoryRequest( const Location& route, std::string& path, const std::string& uri )
{
	std::string idx = ensureTrailingSlash( path ) + route.getIndex();
	std::string body;

	Response* response = new Response( 200, "OK" );

	if (pathExists( idx ) && isRegularFile( idx ) && isReadable( idx ))
	{
		body = readFileContent( idx );
		response->setFileHeaders( body, idx );
	}
	else if (route.getAutoIndex())
	{
		body = generateAutoIndex( path, uri );
		response->setGeneratedHeaders( body, "text/html; charset=utf-8" );
	}
	else
	{
		delete response;
		return (new Response( 403, "Forbidden" ));
	}
	return (response);
}

static Response*	__handleFileRequest( const std::string& path )
{
	std::string body = readFileContent( path );

	Response* response = new Response( 200, "OK" );
	response->setFileHeaders( body, path );
	return (response);
}

Response*	handleHEAD( const ServerConfig& server, const Request& request )
{
	Location route = server.resolveRoute( request );

	if (!server.isMethodAllowed( route, "head" ))
		return (new Response( 405, "Method Not Allowed" ));

	std::string	uri = request.getUri();
	std::string	path = resolvePath( route, uri );

	if (!pathExists( path ))
		return (new Response( 404, "Not Found" ));
	if (!isReadable( path ))
		return (new Response( 403, "Forbidden" ));
	if (isDirectory( path ))
		return (__handleDirectoryRequest( route, path, uri ));
	else
		return (__handleFileRequest( path ));
}
