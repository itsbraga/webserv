/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePOST.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 00:00:23 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/28 21:22:00 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static Response*	__buildPOSTResponse( bool created, const std::string& uri )
{
	Response* response = new Response( created ? 201 : 200, \
									created ? "Created" : "OK" );

	std::string body = created ? "File created\n" : "File updated\n";
	response->setGeneratedContent( body, "text/plain" );
	if (created)
		response->setLocation( uri );
	return (response);
}

static Response*	__uploadPOST( const std::string& path, const std::string& route, std::string& body, std::string& content_type )
{
	std::string uri;

	try {
		uri = handleUpload( body, content_type, path, route );
	}
	catch (const std::exception& e) {
		return (handleHttpException( e ));
	}
	return (__buildPOSTResponse( !uri.empty(), uri ));
}

static Response*	__classicPOST( const std::string& path, const std::string& uri, const std::string& body )
{
	std::string parent_dir = getParentDir( path );

	if (!pathExists( parent_dir ) || !isDirectory( parent_dir ))
		return (new Response( 404, "Not Found" ));
	if (!isWritable( parent_dir ))
		return (new Response( 403, "Forbidden" ));
	if (isDirectory( path ))
		return (new Response( 403, "Forbidden" ));

	bool exists = isRegularFile( path );
	saveFile( path, body );
	return (__buildPOSTResponse( !exists, uri ));
}

static bool	__isValidBodySize( const Request& request, const Location& route )
{
	std::string cl_value = request.getHeaderValue( "content-length" );
	std::string max_config = route.getClientMaxSizeBody();

	size_t body_size = convertBodySize( cl_value );
	size_t max_body_size = max_config.empty() ? DEFAULT_MAX_BODY_SIZE : convertBodySize( max_config );

	return (body_size <= max_body_size);
}

Response*	handlePOST( const ServerConfig& server, const Request& request )
{
	Location route = server.resolveRoute( request );

	if (!server.isMethodAllowed( route, "post" ))
		return (new Response( 405, "Method Not Allowed" ));

	std::string uri = request.getUri();
	std::string path = route.getRoot() + uri;

	if (!isSafePath( route.getRoot(), uri ))
		return (new Response( 403, "Forbidden" ));

	std::string ct_value = request.getHeaderValue( "content-type" );
	std::string body = request.getBody();

	if (!__isValidBodySize( request, route ))
		return (new Response( 413, "Payload Too Large" ));

	if (ct_value.find( "multipart/form-data" ) != std::string::npos)
		return (__uploadPOST( path, uri, body, ct_value ));
	else
		return (__classicPOST( path, uri, body ));
}
