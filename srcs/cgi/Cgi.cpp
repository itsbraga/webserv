/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 20:02:54 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/24 21:46:25 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Response*	cgiHandler( const Request& request, const Server& server )
{
	Response* response = NULL;
	response = checkCgiArguments();
	
}

Response*	checkCgiArguments( )
{
	
}

bool	isCgiRequest( const Request& request, const Server& server )
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