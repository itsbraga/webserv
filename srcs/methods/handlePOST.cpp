/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePOST.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 00:00:23 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/28 15:43:39 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static Response*	__buildPOSTResponse( bool created, const std::string& uri )
{
	Response* response = new Response( created ? 201 : 200, created ? "Created" : "OK" );

	std::string body = created ? "File created\n" : "File updated\n";
	response->setGeneratedContent( body, "text/plain" );
	if (created)
		response->setLocation( uri );
	return (response);
}

static Response*	__uploadPOST( const std::string& path, const std::string& route, std::string& body, std::string& content_type )
{
	std::string uri = handleUpload( body, content_type, path, route );

	return (__buildPOSTResponse( !uri.empty(), uri ));
}

static Response*	__classicPOST( const std::string& path, const std::string& uri, const std::string& body )
{
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

static bool	__isValidBodySize( const Request& request, const Location& route )
{
	size_t body_size = convertBodySize( request.getHeaderValue( "content-length" ) );
	size_t max_body_size = convertBodySize( route.getClientMaxSizeBody() );

	if (body_size > max_body_size)
		return (false);
	
	return (true);
}

Response*	handlePOST( const ServerConfig& server, const Request& request )
{
	Location route = server.resolveRoute( request );

	if (!server.isMethodAllowed( route, "post" ))
		throw MethodNotAllowedException();

	std::string uri = request.getUri();
	std::string path = route.getRoot() + uri;

	if (!isSafePath( route.getRoot(), uri ))
		throw ForbiddenException();

	std::string ct_value = request.getHeaderValue( "content-type" );
	std::string body = request.getBody();

	if (!__isValidBodySize( request, route ))
		throw PayloadTooLargeException();
	if (ct_value.find( "multipart/form-data" ) != std::string::npos)
		return (__uploadPOST( path, uri, body, ct_value ));

	return (__classicPOST( path, uri, body ));
}
