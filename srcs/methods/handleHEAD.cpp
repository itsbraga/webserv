/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleHEAD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:38:47 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/21 02:06:12 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static Response*	__handleDirectoryRequest( Server& server, std::string& path, const std::string& URI )
{
	(void)URI;
	std::string idx = path + (path[path.size() - 1] == '/' ? "" : "/") + server.getIndex();
	std::string	body;

	if (pathExists( idx ) && isRegularFile( idx ) && isReadable( idx ))
	{
		body = readFileContent( idx );
		path = idx;
	}
	// else if (server.getAutoindex())
	// 	body = generateAutoindex( path, URI );
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
	std::string	URI = request.getURI();
	std::string	path = resolvePath( server, URI );

	// Récupérer la location qui match avec l'URI + si la methode est autorisee pour cette route
	// sinon throw MethodNotAllowedException()

	if (!pathExists( path ))
		throw NotFoundException();
	if (!isReadable( path ))
		throw ForbiddenException();
	if (isDirectory( path ))
		return (__handleDirectoryRequest( server, path, URI ));

	return (__handleFileRequest( path ));
}
