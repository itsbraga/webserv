/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleHEAD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:38:47 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/23 20:57:39 by annabrag         ###   ########.fr       */
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

Response*	handleHEAD( Server& server, Request& request )
{
	std::string	uri = request.getUri();
	std::string	path = resolvePath( server, uri );

	// Récupérer la location qui match avec l'uri + si la methode est autorisee pour cette route
	// sinon throw MethodNotAllowedException()

	if (!pathExists( path ))
		throw NotFoundException();
	if (!isReadable( path ))
		throw ForbiddenException();
	if (isDirectory( path ))
		return (__handleDirectoryRequest( server, path, uri ));

	return (__handleFileRequest( path ));
}
