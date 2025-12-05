/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/12/05 02:15:26 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/*
	----------------------------- [ Setters ] ----------------------------
*/
// Si header non trouve ? 
void	Message::setHeaderValue( std::string key, std::string value )
{
	std::vector< std::pair<std::string, std::string> >::iterator it;

	for (it = _headers.begin(); it != _headers.end(); ++it)
	{
		if (it->first == key)
		{
			it->second = value;
			return ;
		}
	}
	_headers.push_back( std::make_pair( key, value ) );
}

void	Message::addHeader( const std::string first, const std::string second )
{
	_headers.push_back( std::make_pair( first, second ) );
}

void	Message::setBody( const std::string body )
{
	_body = body;
	std::string length_str = toString( body.length() );
	std::string contentLength = getHeaderValue( "content-length" );

	if (contentLength.empty() == true)
		addHeader( "content-length", length_str );
	else
		setHeaderValue( "content-length", length_str );
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const std::string	Message::getHeaderMap() const
{
	std::string result;

	for (size_t i = 0; i < _headers.size(); ++i)
		result += _headers[i].first + ": " + _headers[i].second + "\n";
	if (result.empty() == true)
		return ("No headers.\n");
	return (result);
}

const std::string&	Message::getHeaderValue( const std::string& key ) const
{
	std::vector< std::pair<std::string, std::string> >::const_iterator it;

    for (it = _headers.begin(); it != _headers.end(); ++it)
	{
        if (it->first == key)
            return (it->second);
    }
    throw std::runtime_error( "Header not found: " + key );
}
