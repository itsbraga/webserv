/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:25:12 by panther           #+#    #+#             */
/*   Updated: 2025/12/14 00:31:29 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Message.hpp"

/*
	----------------------------- [ Helpers ] ----------------------------
*/
std::string		extractRequestLine( const std::string& serialized )
{
	size_t eol = serialized.find( "\r\n", 0 );
	if (eol == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing CRLF" );

	std::string request_line = serialized.substr( 0, eol );

	if (request_line.empty())
		throw SyntaxErrorException( "400 Bad Request: Empty request line" );
	if (request_line[0] == ' ' || request_line[request_line.size() - 1] == ' ')
		throw SyntaxErrorException( "400 Bad Request: Leading/trailing space" );
	if (request_line.find( '\t' ) != std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: TAB not allowed" );
	if (request_line.find( "  " ) != std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Double space not allowed" );

	return (request_line);
}

void	parseRequestLine( const std::string& request_line, std::string& method, std::string& URI, std::string& protocol_version )
{
	std::stringstream	ss( request_line );

	if (!(ss >> method >> URI >> protocol_version))
		throw SyntaxErrorException( "400 Bad Request: Incomplete request line" );

	std::string extra;
	if (ss >> extra)
		throw SyntaxErrorException( "400 Bad Request: Too many elements in request line" );
}

void	validateMethod( const std::string& method )
{
	for (size_t i = 0; i < method.size(); i++)
	{
		if (method[i] < 'A' || method[i] > 'Z')
			throw SyntaxErrorException( "400 Bad Request: Method not in uppercase" );
	}

	if (method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD")
		throw SyntaxErrorException( "501 Not Implemented" ); // Erreur peut-être formulée ailleurs
}

void	validateURI( const std::string& URI )
{
	if (URI.empty() || URI[0] != '/')
		throw SyntaxErrorException( "400 Bad Request: URI must start with '/'" );
}

void	validateProtocolVersion( const std::string& protocol_version )
{
	if (protocol_version != "HTTP/1.1")
		throw SyntaxErrorException( "505 HTTP Version Not Supported" ); // changer d'exception
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
		throw SyntaxErrorException( "400 Bad Request: Missing CRLF after request line" );
	header_start += 2;

	header_end = serialized.find( "\r\n\r\n", header_start );
	if (header_end == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing double CRLF (end of headers)" );

	return (header_end);
}