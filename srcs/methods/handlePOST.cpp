/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePOST.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 00:00:23 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/23 21:19:09 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__resolvePath( Server& server, const std::string& uri )
{
	return (server.getRoot() + uri);
}

static Response*	__buildPOSTResponse( bool created, const std::string& uri )
{
	Response* response = new Response( created ? 201 : 200, created ? "Created" : "OK" );

	response->setBody( created ? "File created\n" : "File updated\n" );
	response->setContentLength( "13" );
	response->setContentType( "text/plain" );
	if (created)
		response->setLocation( uri );

	return (response);
}

// static Response*	__uploadPOST( Server& server, Location& location, std::string& body, std::string& content_type )
// {
// 	std::string dir = server.getRoot() + location.getUri();
// 	std::string route = location.getUri();
// 	std::string uri = handleUpload( body, content_type, dir, route );

// 	return (__buildPOSTResponse( !uri.empty(), uri ));
// }

static Response*	__uploadPOST( Server& server, std::string& body, std::string& content_type )
{
	(void)server;

	std::string dir = "./www/uploads";
	std::string route = "/uploads";
	std::string uri = handleUpload( body, content_type, dir, route );

	return (__buildPOSTResponse( !uri.empty(), uri ));
}

static Response*	__classicPOST( Server& server, std::string& uri, std::string& body )
{
	std::string path = __resolvePath( server, uri );
	std::string parent_dir = getParentDir( path );

	if (!pathExists( parent_dir ) || !isDirectory( parent_dir ))
		throw NotFoundException();
	if (!isWritable( parent_dir ))
		throw ForbiddenException();
	if (isDirectory( path ))
		throw ForbiddenException();

	bool exists = isRegularFile( path );
	saveFile( path, body );

	return (__buildPOSTResponse( !exists, uri ));
}

Response*	handlePOST( Server& server, Request& request )
{
	std::string uri = request.getUri();

	if (!isSafePath( server.getRoot(), uri )) // modifier getRoot quand Location OK
		throw ForbiddenException();

	// 1/ Récupérer la location qui match avec l'uri, si pas trouvée voir dans bloc server en lui-même
	// 2/ Check si POST est autorisé pour cette route, sinon throw MethodNotAllowedException()
	// 3/ Check client_max_body_size (comme pour l'uri)

	std::string ct_value = request.getHeaderValue( "Content-Type" );
	std::string body = request.getBody();

	if (ct_value.find( "multipart/form-data" ) != std::string::npos)
		return (__uploadPOST( server, body, ct_value ));

	return (__classicPOST( server, uri, body ));
}
