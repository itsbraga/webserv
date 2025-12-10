/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/12/10 20:43:49 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/*
	----------------------------- [ Helpers ] ----------------------------
*/
static std::string	__toLower( const std::string& str )
{
	if (str.empty())
		return (str);

	std::string result = str;

	for (size_t i = 0; i < result.size(); i++)
		result[i] = std::tolower( static_cast<unsigned char>( result[i] ) );
	return (result);
}

void	Message::addHeader( const std::string first, const std::string second )
{
	if (first.empty() && second.empty())
	{
		std::cerr << ERR_PREFIX << "empty header_name, empty header_value" << std::endl;
		return ;
	}
		
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
	if (line.empty())
		throw SyntaxErrorException( "empty line" );

	size_t colonPos = line.find( ':' );
	if (colonPos == std::string::npos || colonPos == 0)
		throw SyntaxErrorException( "400 Bad Request: Invalid header format" );

	std::string name = line.substr( 0, colonPos);
	if (name.find( ' ' ) != std::string::npos || name.find( '\t' ) != std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Space in header name" );

	size_t valueStart = colonPos + 1;
	while (valueStart < line.size() && (line[valueStart] == ' ' || line[valueStart] == '\t'))
		valueStart++;

	std::string value = line.substr( valueStart );

	size_t valueEnd = value.find_last_not_of(" \t");
	if (valueEnd != std::string::npos)
		value = value.substr( 0, valueEnd + 1 );
	
	return (std::make_pair( name, value ));
}

void	Message::_unchunkBody( std::string body )
{
	if (body.empty())
		throw SyntaxErrorException( "empty body" );
}

/*
	----------------------------- [ Setters ] ----------------------------
*/
// Si header non trouve ? 
void	Message::setHeaderValue( std::string key, std::string value )
{
	if (key.empty())
		return ;

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

void	Message::setBody( const std::string body )
{
	if (body.empty())
		return ;

	_body = body;

	std::string length_str = toString( body.length() );
	std::string contentLength = getHeaderValue( "content-length" );

	if (contentLength.empty())
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
	if (result.empty())
		return ("No header found\n");
	return (result);
}

const std::string&	Message::getHeaderValue( const std::string& key ) const
{
	if (key.empty())
		return ("empty key");

	std::string lowerKey = __toLower(key);

	std::vector< std::pair<std::string, std::string> >::const_iterator it;

    for (it = _headers.begin(); it != _headers.end(); ++it)
	{
        if (__toLower( it->first ) == lowerKey)
            return (it->second);
    }
    throw std::runtime_error( "header not found: " + key );
}