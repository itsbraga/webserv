/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:25:12 by panther           #+#    #+#             */
/*   Updated: 2025/12/24 17:41:49 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------------- [ Utilities ] ---------------------------
*/
std::string		extractRequestLine( const std::string& serialized )
{
	size_t eol = serialized.find( "\r\n", 0 );
	if (eol == std::string::npos)
		throw BadRequestException( "Missing CRLF" );

	std::string request_line = serialized.substr( 0, eol );
	if (request_line.empty())
		throw BadRequestException( "Empty request line" );

	if (request_line[0] == ' ' || request_line[request_line.size() - 1] == ' ')
		throw BadRequestException( "Leading/trailing space" );
	if (request_line.find( '\t' ) != std::string::npos)
		throw BadRequestException( "TAB not allowed" );
	if (request_line.find( "  " ) != std::string::npos)
		throw BadRequestException( "Double space not allowed" );

	return (request_line);
}

void	parseRequestLine( const std::string& request_line, std::string& method, std::string& uri, std::string& protocol_version )
{
	std::stringstream ss( request_line );

	if (!(ss >> method >> uri >> protocol_version))
		throw BadRequestException( "Incomplete request line" );

	std::string extra;
	if (ss >> extra)
		throw BadRequestException( "Too many elements in request line" );
}

void	validateMethod( const std::string& method )
{
	for (size_t i = 0; i < method.size(); i++)
	{
		if (method[i] < 'A' || method[i] > 'Z')
			throw BadRequestException( "Method not in uppercase" );
	}

	if (method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD")
		throw NotImplementedException(); // Peut-être ailleurs
}

void	validateUri( const std::string& uri )
{
	if (uri.empty() || uri[0] != '/')
		throw BadRequestException( "uri must start with '/'" );
}

void	validateProtocolVersion( const std::string& protocol_version )
{
	if (protocol_version != "HTTP/1.1")
		throw HTTPVersionNotSupportedException(); // Peut-être ailleurs
}

bool	isValidHeaderName( const std::string& name )
{
	const std::string allowed = "!#$%&'*+-.^_`|~";

	for (size_t i = 0; i < name.size(); i++)
	{
		char c = name[i];
		if (!std::isalnum( static_cast<unsigned char>( c ) ) && allowed.find( c ) == std::string::npos)
			return (false);
	}

	return (true);
}

size_t	findHeaderBoundaries( const std::string& serialized, size_t& header_start, size_t& header_end )
{
	header_start = serialized.find( "\r\n" );
	if (header_start == std::string::npos)
		throw BadRequestException( "Missing CRLF after request line" );
	header_start += 2;

	header_end = serialized.find( "\r\n\r\n", header_start );
	if (header_end == std::string::npos)
		throw BadRequestException( "Missing double CRLF - end of headers" );

	return (header_end);
}
