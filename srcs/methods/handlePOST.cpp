/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePOST.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 00:00:23 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/18 22:15:18 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__resolvePath( Server& server, const std::string& URI )
{
	return (server.getRoot() + URI);
}

static std::string	__getParentDir( const std::string& path )
{
	size_t pos = path.find_last_of( '/' );
	if (pos == std::string::npos)
		return (".");

	return (path.substr( 0, pos ));
}

static Response*	__uploadPOST( Server& server, std::string& body, std::string& content_type )
{
	(void)server; // retirer quand Location sera OK

	std::string dir = "./www/upload";	// a changer: recuperer depuis fichier de config
	std::string route = "/upload";		// a changer aussi
	std::string URI = handleUpload( body, content_type, dir, route );

	Response* response = new Response( URI.empty() ? 200 : 201, URI.empty() ? "OK" : "Created" );

	response->setBody( URI.empty() ? "File updated\n" : "File created\n" );
	response->setContentLength( "13" );
	response->setContentType( "text/plain" );
	if (!URI.empty())
		response->setLocation( URI );
	return (response);
}

static Response*	__classicPOST( Server& server, Request& request, std::string& body )
{
	std::string URI = request.getURI();
	std::string path = __resolvePath( server, URI );
	std::string parent_dir = __getParentDir( path );

	if (!fileExists( parent_dir ) || !isDirectory( parent_dir ))
		throw NotFoundException();
	if (!isWritable( parent_dir ))
		throw ForbiddenException();

	bool exists = fileExists( path );
	saveFile( path, body );

	Response* response = new Response( exists ? 200 : 201, exists ? "OK" : "Created" );

	response->setBody( exists ? "File updated\n" : "File created\n" );
	response->setContentLength( "13" );
	response->setContentType( "text/plain" );
	if (!exists)
		response->setLocation( URI );
	return (response);
}

Response*	handlePOST( Server& server, Request& request )
{
	std::string ct_value = request.getHeaderValue( "Content-Type" );
	std::string body = request.getBody();
	if (body.empty())
		throw BadRequestException();

	if (ct_value.find( "multipart/form-data" ) != std::string::npos)
		return (__uploadPOST( server, body, ct_value ));
	return (__classicPOST( server, request, body ));
}
