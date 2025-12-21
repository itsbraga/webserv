/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:07:51 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/21 01:44:26 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__resolvePath( Server& server, const std::string& URI )
{
	return (server.getRoot() + URI);
}

Response*	handleDELETE( Server& server, Request& request )
{
	std::string	URI = request.getURI();

	if (URI == "/")
		throw ForbiddenException();
	if (!isSafePath( server.getRoot(), URI )) // modifier getRoot quand Location OK
		throw ForbiddenException();

	// Récupérer la location qui match avec l'URI + si la methode est autorisee pour cette route
	// sinon throw MethodNotAllowedException()

	std::string	path = __resolvePath( server, URI );

	if (!pathExists( path ))
		throw NotFoundException();
	if (isDirectory( path ))
		throw ForbiddenException();
	if (!isWritable( getParentDir( path ) ))
		throw ForbiddenException();
	if (!deleteFile( path ))
		throw InternalServerErrorException();

	return (new Response( 200, "OK" )); // 204 No Content ou 200 OK
}
