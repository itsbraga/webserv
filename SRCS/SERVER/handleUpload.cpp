/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUpload.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 15:56:09 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/17 16:09:12 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Request.hpp"

bool	Server::handleUpload( Request& request )
{
	if (request.getMethod() == "POST")
	{
		std::string cl_value = request.getHeaderValue( "Content-Type" );
		// std::string target = cl_value.find( "boundary=" );
	}
}