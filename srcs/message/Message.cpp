/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/12/28 20:01:42 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	----------------------------- [ Helper ] -----------------------------
*/
static int	__hexToInt( const std::string& hex )
{
	int result = 0;

	for (size_t i = 0; i < hex.size(); ++i)
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
bool	Message::_checkProhibitedDuplicate( const std::string& name )
{
	static std::set<std::string> uniqueHeaders;
	if (uniqueHeaders.empty())
	{
		uniqueHeaders.insert("host");
		uniqueHeaders.insert("content-type");
		uniqueHeaders.insert("content-length");
		uniqueHeaders.insert("transfer-encoding");
		uniqueHeaders.insert("authorization");
		uniqueHeaders.insert("from");
		uniqueHeaders.insert("referer");
		uniqueHeaders.insert("user-agent");
		uniqueHeaders.insert("max-forwards");
		uniqueHeaders.insert("if-match");
		uniqueHeaders.insert("if-none-match");
		uniqueHeaders.insert("if-modified-since");
		uniqueHeaders.insert("if-unmodified-since");
		uniqueHeaders.insert("if-range");
		uniqueHeaders.insert("range");
	}
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
	
	lowerStr( name );

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
			break;

		size_t data_start = line_end + 2;
		if (data_start + chunk_size > chunked_data.size())
			throw BadRequestException( "Incomplete chunk" );

		result.append( chunked_data, data_start, chunk_size );
		pos = data_start + chunk_size + 2;
	}
	_body = result;
}

/*
	---------------------------- [ Setters ] -----------------------------
*/
void	Message::setBody( const std::string& body )
{
	_body = body;
}

void	Message::addHeader( const std::string& key, const std::string& value )
{
	if (key.empty() || value.empty())
		return ;

	_headers.insert( std::make_pair(key, value) );
}

void	Message::addHeader( std::pair<std::string, std::string> header )
{
	_headers.insert( header );
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const std::string	Message::getHeaderValue( const std::string& key ) const
{
	if (key.empty())
		return ("");

	std::string lower_key = toLower( key );
	std::multimap<std::string, std::string>::const_iterator it = _headers.find( lower_key );

	if (it != _headers.end())
		return (it->second);
	return ("");
}

/*
	----------------------------- [ Checker ] ----------------------------
*/
bool	Message::hasHeader( const std::string& key ) const
{
	if (key.empty())
		return (false);

	std::string lower_key = toLower( key );
	std::multimap<std::string, std::string>::const_iterator it = _headers.find( lower_key );

	if (it != _headers.end())
		return (true);
	return (false);
}

/*
	----------------------------- [ Print ] ------------------------------
*/
const std::string	Message::printHeaderMap() const
{
	std::string result;
	std::multimap<std::string, std::string>::const_iterator it = _headers.begin();

	for (; it != _headers.end(); ++it)
		result += it->first + ": " + it->second + "\n";

	if (result.empty())
		return (ERR_PREFIX "No header found\n");
	return (result);
}