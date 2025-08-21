/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/21 21:10:14 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Request::Request() : Message() {}

Request::Request( const std::string& serialized ) : Message()
{
	deserializeRequest(serialized);
}

Request::~Request() {}

/*
	----------------------------- [ Parsing ] ----------------------------
*/
void	Request::deserializeRequest( const std::string& serialized )
{
	std::stringstream	ss(serialized);
	std::string			requestLine;

	size_t pos = serialized.find("\r\n", 0);
	if (pos != std::string::npos)
		requestLine = serialized.substr(0, pos);
	else
		requestLine = serialized;
}

/*
	----------------------------- [ Setters ] ----------------------------
*/


/*
	----------------------------- [ Getters ] ----------------------------
*/

