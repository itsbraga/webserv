/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/12/14 01:24:43 by panther          ###   ########.fr       */
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

static int	__hexToInt( const std::string& hex )
{
	int result = 0;

	for (size_t i = 0; i < hex.size(); i++)
	{
		char c = hex[i];
		result *= 16;

		if (c >= '0' && c <= '9')
			result += c - '0';
		else if (c >= 'a' && c <= 'f')
			result += c - 'a' + 10;
		else if (c >= 'A' && c <= 'F')
			result += c - 'A' + 10;
		else
			return (-1);
	}
	return (result);
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

/*
	[rfc2616]

	3.6.1 Chunked Transfer Coding

	The chunked encoding modifies the body of a message in order to
	transfer it as a series of chunks, each with its own size indicator,
	followed by an OPTIONAL trailer containing entity-header fields.

	The chunk-size field is a string of hex digits indicating the size of
	the chunk. The chunked encoding is ended by any chunk whose size is
	zero, followed by the trailer, which is terminated by an empty line.

	The trailer allows the sender to include additional HTTP header
	fields at the end of the message.

*/
void	Message::_unchunkBody( const std::string& chunked_data )
{
	std::string result;
	size_t pos = 0;

	while (pos < chunked_data.size())
	{
		size_t line_end = chunked_data.find( "\r\n", pos );
		if (line_end == std::string::npos)
			throw SyntaxErrorException( "400 Bad Request: Malformd chunked body" );
	
		std::string line_size = chunked_data.substr( pos, line_end - pos );

		size_t semicolon = line_size.find( ';' );
		if (semicolon != std::string::npos)
			line_size = line_size.substr( 0, semicolon );

		int chunk_size = __hexToInt( line_size );
		if (chunk_size < 0)
			throw SyntaxErrorException( "400 Bad Request: Invalid chunk size" );
		if (chunk_size == 0) // end chunk
			break ;

		size_t data_start = line_end + 2;
		if (data_start + chunk_size > chunked_data.size())
			throw SyntaxErrorException( "400 Bad Request: Incomplete chunk" );

		result.append( chunked_data, data_start, chunk_size );
		pos = data_start + chunk_size + 2;
	}

	_body = result;
}

/*
	-------------------------- [ Setter tool ] ---------------------------
*/
void	Message::addHeader( const std::string& key, const std::string& value )
{
	if (key.empty() || value.empty())
		return ;
	_headers.push_back( std::make_pair( key, value ) );
}

/*
	----------------------------- [ Setters ] ----------------------------
*/
void	Message::setHeaderValue( const std::string& key, const std::string& value )
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
	addHeader( key, value );
}

void	Message::setBody( const std::string& body )
{
	_body = body;

	std::string length_str = toString( _body.length() );
	setHeaderValue( "Content-Length", length_str );
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
		return (ERR_PREFIX "No header found\n");
	return (result);
}

const std::string	Message::getHeaderValue( const std::string& key ) const
{
	if (key.empty())
		return (ERR_PREFIX "empty key");

	std::string lower_key = __toLower(key);

	std::vector< std::pair<std::string, std::string> >::const_iterator it;

    for (it = _headers.begin(); it != _headers.end(); ++it)
	{
        if (__toLower( it->first ) == lower_key)
            return (it->second);
    }
    return (ERR_PREFIX "Header not found: " + key);
}