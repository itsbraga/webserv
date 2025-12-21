/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePOST.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 00:00:23 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/21 01:54:02 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__resolvePath( Server& server, const std::string& URI )
{
	return (server.getRoot() + URI);
}

static Response*	__buildPOSTResponse( bool created, const std::string& URI )
{
	Response* response = new Response( created ? 201 : 200, created ? "Created" : "OK" );

	response->setBody( created ? "File created\n" : "File updated\n" );
	response->setContentLength( "13" );
	response->setContentType( "text/plain" );
	if (created)
		response->setLocation( URI );

	return (response);
}

static Response*	__uploadPOST( Server& server, std::string& body, std::string& content_type )
{
	(void)server; // retirer quand Location sera OK

	std::string dir = "./www/uploads";	// a changer: recuperer depuis fichier de config
	std::string route = "/uploads";		// a changer aussi
	std::string URI = handleUpload( body, content_type, dir, route );

	return (__buildPOSTResponse( !URI.empty(), URI ));
}

static Response*	__classicPOST( Server& server, std::string& URI, std::string& body )
{
	std::string path = __resolvePath( server, URI );
	std::string parent_dir = getParentDir( path );

	if (!pathExists( parent_dir ) || !isDirectory( parent_dir ))
		throw NotFoundException();
	if (!isWritable( parent_dir ))
		throw ForbiddenException();
	if (isDirectory( path ))
		throw ForbiddenException();

	bool exists = isRegularFile( path );
	saveFile( path, body );

	return (__buildPOSTResponse( !exists, URI ));
}

Response*	handlePOST( Server& server, Request& request )
{
	std::string URI = request.getURI();

	if (!isSafePath( server.getRoot(), URI )) // modifier getRoot quand Location OK
		throw ForbiddenException();

	// 1/ Récupérer la location qui match avec l'URI
	// 2/ Check si POST est autorisé pour cette route, sinon throw MethodNotAllowedException()
	// 3/ Check client_max_body_size

	std::string ct_value = request.getHeaderValue( "Content-Type" );
	std::string body = request.getBody();

	if (ct_value.find( "multipart/form-data" ) != std::string::npos)
		return (__uploadPOST( server, body, ct_value ));

	return (__classicPOST( server, URI, body ));
}
