/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:07:51 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/23 17:34:45 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__resolvePath( Server& server, const std::string& uri )
{
	return (server.getRoot() + uri);
}

Response*	handleDELETE( Server& server, Request& request )
{
	std::string	uri = request.getUri();

	if (uri == "/")
		throw ForbiddenException();
	if (!isSafePath( server.getRoot(), uri )) // modifier getRoot quand Location OK
		throw ForbiddenException();

	// Récupérer la location qui match avec l'URI + si la methode est autorisee pour cette route
	// sinon throw MethodNotAllowedException()

	std::string	path = __resolvePath( server, uri );

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
