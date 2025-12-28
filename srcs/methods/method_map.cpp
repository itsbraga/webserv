/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_map.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:02:09 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/28 20:19:01 by annabrag         ###   ########.fr       */
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
	Response* response = NULL;

	if (it != g_method_map.end())
		response = (*it->second)( server, request );
	else
		response = new Response( 501, "Not Implemented" );
	// ErrorPageHandler(*response, request, server);
	return (response);
}

Response*	handleHttpException( const std::exception& e )
{
	const HttpException* http = dynamic_cast<const HttpException*>( &e );

	if (http)
		return (new Response( http->getCode(), http->getStatus() ));
	else
		return (new Response( 500, "Internal Server Error" ));
}
