/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/12/02 17:24:33 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	----------------------------- [ Setters ] ----------------------------
*/
// Si header non trouve ? 
void	Message::setHeaderValue( std::string key, std::string value )
{
	std::vector< std::pair<std::string, std::string> >::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); ++it)
	{
		if (it->first == key)
		{
			it->second = value;
			return ;
		}
	}
	this->_headers.push_back(std::make_pair(key, value));
}

void	Message::addHeader( const std::string first, const std::string second )
{
	this->_headers.push_back(std::make_pair(first, second));
}

void	Message::setBody( const std::string body )
{
	this->_body = body;
	std::string length_str = toString(body.length());
	std::string contentLength = getHeaderValue("content-length");

	if (contentLength.empty() == true)
		addHeader("content-length", length_str);
	else
		setHeaderValue("content-length", length_str);
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const std::string&	Message::getHeaderMap() const
{
	std::string result;

	for (size_t i = 0; i < this->_headers.size(); ++i)
		result += this->_headers[i].first + ": " + this->_headers[i].second + "\n";
	if (result.empty() == true)
		return ("No headers.\n");
	return (result);
}

const std::string&	Message::getHeaderValue( const std::string& key ) const
{
	std::vector< std::pair<std::string, std::string> >::const_iterator it;

    for (it = this->_headers.begin(); it != this->_headers.end(); ++it)
	{
        if (it->first == key)
            return (it->second);
    }
    throw std::runtime_error("Header not found: " + key);
}
