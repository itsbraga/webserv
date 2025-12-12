/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/12/12 16:38:45 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/*
	----------------------------- [ Helpers ] ----------------------------
*/
static std::string	__toLower( const std::string& str )
{
	std::string result = str;

	for (size_t i = 0; i < result.size(); i++)
		result[i] = std::tolower( static_cast<unsigned char>( result[i] ) );
	return (result);
}

void	Message::addHeader( const std::string& first, const std::string& second )
{	
	_headers.push_back( std::make_pair( first, second ) );
}

/*
	------------------------ [ Private methods ] -------------------------
*/
bool	Message::_hasHeader( const std::string& key ) const
{
	std::string lowerKey = __toLower( key );

	std::vector< std::pair<std::string, std::string> >::const_iterator	it;

	for (it = _headers.begin(); it != _headers.end(); ++it)
	{
		if (__toLower( it->first ) == lowerKey)
			return (true);
	}
	return (false);
}

std::pair<std::string, std::string>		Message::_parseHeaderLine( const std::string& line ) const
{
	size_t colonPos = line.find( ':' );
	if (colonPos == std::string::npos || colonPos == 0)
		throw SyntaxErrorException( "400 Bad Request: Invalid header format" );

	std::string name = line.substr( 0, colonPos);
	if (name.find( ' ' ) != std::string::npos || name.find( '\t' ) != std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Space in header name" );

	size_t value_start = colonPos + 1;
	while (value_start < line.size() && (line[value_start] == ' ' || line[value_start] == '\t'))
		value_start++;

	std::string value = line.substr( value_start );

	size_t value_end = value.find_last_not_of(" \t");
	if (value_end != std::string::npos)
		value = value.substr( 0, value_end + 1 );
	
	return (std::make_pair( name, value ));
}

void	Message::_unchunkBody( std::string body )
{
	if (body.empty())
		throw std::invalid_argument( "_unchunkBody(): empty body" );
}

/*
	----------------------------- [ Setters ] ----------------------------
*/
// Si header non trouve ? 
void	Message::setHeaderValue( const std::string& key, const std::string& value )
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

void	Message::setBody( const std::string& body )
{
	_body = body;

	std::string length_str = toString( _body.length() );

	if (_hasHeader( "Content-Length" ))
		setHeaderValue( "Content-Length", length_str );
	else
		addHeader( "Content-Length", length_str );
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const std::string	Message::getHeaderMap() const
{
	std::string result;

	for (size_t i = 0; i < _headers.size(); ++i)
		result += _headers[i].first + ": " + _headers[i].second + "\n";
	if (result.empty())
		return ("No header found\n");

	return (result);
}

const std::string&	Message::getHeaderValue( const std::string& key ) const
{
	std::string lower_key = __toLower(key);

	std::vector< std::pair<std::string, std::string> >::const_iterator it;

    for (it = _headers.begin(); it != _headers.end(); ++it)
	{
        if (__toLower( it->first ) == lower_key)
            return (it->second);
    }
    throw std::runtime_error( "header not found: " + key );
}