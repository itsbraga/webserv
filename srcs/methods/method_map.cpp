/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_map.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:02:09 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/25 18:38:09 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::map<std::string, MethodHandler> g_method_map;

void	init_method_map()
{
	g_method_map["GET"] = &handleGET;
	g_method_map["POST"] = &handlePOST;
	g_method_map["HEAD"] = &handleHEAD;
	g_method_map["DELETE"] = &handleDELETE;
}

Response*	handleMethod( const ServerConfig& server, const Request& request )
{
	std::map<std::string, MethodHandler>::const_iterator it = g_method_map.find( request.getMethod() );

	if (it != g_method_map.end())
		return ((*it->second)( server, request ));

	return (new Response( 501, "Not Implemented" ));
}

Response*	handleHttpException( const std::exception& e )
{
	const HttpException* http = dynamic_cast<const HttpException*>( &e );
	if (http)
		return (new Response( http->getCode(), http->getStatus() ));

	return (new Response( 500, "Internal Server Error" ));
}
