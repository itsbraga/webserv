/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_map.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:02:09 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/18 18:35:33 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::map<std::string, MethodHandler> g_method_map;

void	init_method_map()
{
	g_method_map["GET"] = &handleGET;
	g_method_map["POST"] = &handlePOST;
	// g_method_map["HEAD"] = &handleHEAD;
	// g_method_map["DELETE"] = &handleDELETE;
}

Response*	handleMethod( Server& server, Request& request )
{
	std::map<std::string, MethodHandler>::const_iterator it;

	it = g_method_map.find( request.getMethod() );
	if (it != g_method_map.end())
		return ((*it->second)( server, request ));

	return (new Response( 501, "Not Implemented" ));
}
