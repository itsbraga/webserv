/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:07:51 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/24 18:43:25 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Response*	handleDELETE( Server& server, Request& request )
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

	// setBody --> Fichier "blabla" supprimé.
	return (new Response( 200, "OK" )); // 204 No Content ou 200 OK
}
