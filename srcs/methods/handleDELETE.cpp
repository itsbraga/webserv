/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:07:51 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/29 12:54:02 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Response*	handleDELETE( const ServerConfig& server, const Request& request )
{
	Location route = server.resolveRoute( request );
	std::string root = route.getRoot();

	if (!server.isMethodAllowed( route, "DELETE" ))
		return (new Response( 405, "Method Not Allowed" ));

	std::string	uri = extractPathFromUri( request.getUri() );
	if (uri == "/")
		return (new Response( 403, "Forbidden" ));
	if (!isSafePath( root, uri ))
		return (new Response( 403, "Forbidden" ));

	std::string	path = root + uri;
	if (!pathExists( path ))
		return (new Response( 404, "Not Found" ));
	if (isDirectory( path ))
		return (new Response( 403, "Forbidden" ));
	if (!isWritable( getParentDir( path ) ))
		return (new Response( 403, "Forbidden" ));
	if (!deleteFile( path ))
		return (new Response( 500, "Internal Server Error" ));

	Response* response = new Response( 200, "OK" ); // 204 ou 200
	response->setGeneratedContent( "File " + uri + " deleted\n", "text/plain" );
	return (response);
}
