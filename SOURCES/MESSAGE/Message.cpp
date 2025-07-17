/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/17 21:17:00 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Message::Message() : _http_version("1.1") {}

/*
	----------------------------- [ Setters ] ----------------------------
*/
void	Message::setHeaderMap( const std::string __attribute_maybe_unused__ headermap )
{

}

// Si header non trouve ? 
void	Message::setHeaderValue( std::string key, std::string value )
{
	std::vector< std::pair<std::string, std::string > >::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); ++it)
	{
		if (it->first == key)
			it->second = value;
	}
}

void	Message::addHeader( const std::string first, const std::string second )
{
	_headers.push_back(std::make_pair(first, second));
}

void	Message::setBody( const std::string body )
{
	this->_body = body;
	std::string length_str = toString(body.length());
	
	std::string ContentLength = getHeaderValue("content-length");
	if (ContentLength.empty())
		addHeader("content-length", length_str);
	else
		setHeaderValue("content-length", length_str);
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const std::string&		Message::getHttpVersion() const
{
	return (this->_http_version);
}

std::string		Message::getHeaderMap() const
{
	std::string result;
	for (size_t i = 0; i < this->_headers.size(); ++i)
		result += this->_headers[i].first + ": " + this->_headers[i].second + "\n";
	if (result.empty())
		return ("No headers.\n");
	return (result);
}

std::string		Message::getHeaderValue( std::string key ) const
{
	std::vector< std::pair< std::string, std::string > >::const_iterator it;

	it = this->_headers.begin();
    for (; it != this->_headers.end(); ++it)
	{
        if (it->first == key)
            return (it->second);
    }
    return ("");
}

std::string		Message::getBody() const
{
	return (this->_body);
}
