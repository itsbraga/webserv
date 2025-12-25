/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:07:51 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/25 18:37:35 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Response*	handleDELETE( const ServerConfig& server, const Request& request )
{
	Location route = server.resolveRoute( request );
	std::string root = route.getRoot();

	if (!server.isMethodAllowed( route, "DELETE" ))
		throw MethodNotAllowedException();

	std::string	uri = request.getUri();
	if (uri == "/")
		throw ForbiddenException();

	if (!isSafePath( root, uri ))
		throw ForbiddenException();

	std::string	path = root + uri;

	if (!pathExists( path ))
		throw NotFoundException();
	if (isDirectory( path ))
		throw ForbiddenException();
	if (!isWritable( getParentDir( path ) ))
		throw ForbiddenException();
	if (!deleteFile( path ))
		throw InternalServerErrorException();

	Response* response = new Response( 200, "OK" ); // 204 ou 200
	response->loadContent( "File " + uri + " deleted", path ); // setBody --> Fichier "blabla" supprimé.

	return (response);
}
