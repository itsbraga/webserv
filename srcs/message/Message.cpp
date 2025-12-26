/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/12/25 22:42:31 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------------- [ Utilities ] ---------------------------
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
bool	Message::hasHeader( const std::string& key ) const
{
	if (key.empty())
		return (false);

	std::string lower_key = __toLower(key);
	std::multimap<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it != _headers.end())
		return (true);
    return (false);
}

bool	Message::_checkProhibitedDuplicate( const std::string& name )
{
	static const std::set<std::string> uniqueHeaders = {
        "host", "content-type", "content-length", "transfer-encoding",
        "authorization", "from", "referer", "user-agent", "max-forwards", "if-match", "if-none-match",
        "if-modified-since", "if-unmodified-since", "if-range", "range"
    };
	if (uniqueHeaders.count( name ) == 1)
	{
		if (_headers.count( name ) == 1)
			return (true);
	}
	return (false);
}

std::pair<std::string, std::string>		Message::_parseHeaderLine( const std::string& line ) const
{
	size_t colonPos = line.find( ':' );
	if (colonPos == std::string::npos || colonPos == 0)
		throw BadRequestException( "Invalid header format" );

	std::string name = line.substr( 0, colonPos);
	if (name.empty())
		throw BadRequestException( "No header name" );
	if (name.find( ' ' ) != std::string::npos || name.find( '\t' ) != std::string::npos)
		throw BadRequestException( "Space in header name" );
	
	name = __toLower( name );

	size_t value_start = colonPos + 1;
	while (value_start < line.size() && (line[value_start] == ' ' || line[value_start] == '\t'))
		value_start++;

	std::string value = line.substr( value_start );
	if (value.empty())
		throw BadRequestException( "No header value" );

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
			throw BadRequestException( "Malformed chunked body" );
	
		std::string line_size = chunked_data.substr( pos, line_end - pos );
		if (line_size.empty())
			throw BadRequestException( "No chunk size" );

		size_t semicolon = line_size.find( ';' );
		if (semicolon != std::string::npos)
			line_size = line_size.substr( 0, semicolon );

		int chunk_size = __hexToInt( line_size );
		if (chunk_size < 0)
			throw BadRequestException( "Invalid chunk size" );
		if (chunk_size == 0) // end chunk
			break ;

		size_t data_start = line_end + 2;
		if (data_start + chunk_size > chunked_data.size())
			throw BadRequestException( "Incomplete chunk" );

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

	_headers.insert( std::make_pair(key, value) );
}

/*
	----------------------------- [ Setters ] ----------------------------
*/
void	Message::setHeaderValue( const std::string& key, const std::string& value )
{
	if (key.empty())
		return ;

	std::vector< std::pair<std::string, std::string> >::iterator it = _headers.begin();

	for (; it != _headers.end(); ++it)
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
}

/*
	----------------------------- [ Getters ] ----------------------------
*/

const std::string	Message::getHeaderValue( const std::string& key ) const
{
	if (key.empty())
		return ("");

	std::string lower_key = __toLower(key);
	std::multimap<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it != _headers.end())
		return (it->second);
    return (ERR_PREFIX "Header not found: " + key);
}