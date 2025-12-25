/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 20:02:54 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/25 18:39:10 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

// Response*	cgiHandler( const Request& request, const ServerConfig& server )
// {
// 	Response* response = NULL;
// 	response = checkCgiArguments();
	
// }

// Response*	checkCgiArguments( )
// {
	
// }

bool	isCgiRequest( const Request& request, const ServerConfig& server )
{
	std::map<std::string, Location>::const_iterator it;
	
	it = server.findMatchingLocation(request); 
	if (it != server.getLocations().end())
	{
		if (it->second.getCgiBin().empty() == false)
			return (true);	
	}
	return (false);
}