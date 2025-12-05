/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/05 20:17:00 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Request::Request( const std::string& serialized ) : Message()
{
	deserializeRequest( serialized );
}

/*
	----------------------------- [ Helpers ] ----------------------------
*/
static std::string	__rawRequestLineVerif( const std::string& serialized )
{
	size_t eol = serialized.find( "\r\n", 0 );
	if (eol == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing CRLF" );

	std::string requestLine = serialized.substr( 0, eol );

	if (requestLine.empty())
		throw SyntaxErrorException( "400 Bad Request: Empty request line" );
	if (requestLine[0] == ' ' || requestLine[requestLine.size() - 1] == ' ')
		throw SyntaxErrorException( "400 Bad Request: Leading/trailing space" );
	if (requestLine.find( '\t' ) != std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: TAB not allowed" );
	if (requestLine.find( "  " ) != std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Double space not allowed" );
	return (requestLine);
}

static bool		__isValidHeaderName( const std::string& name )
{
	if (name.empty())
		return (false);
}

/*
	----------------------------- [ Parsing ] ----------------------------

	REMPLACER les SyntaxErrorException par les ResponseStatus
*/
void	Request::requestLineCheck( const std::string& serialized )
{
	std::string requestLine = __rawRequestLineVerif( serialized );

	std::stringstream	ss( requestLine );
	std::string			method, URI, protocolVersion;

	if (!(ss >> method >> URI >> protocolVersion))
		throw SyntaxErrorException( "400 Bad Request: Malformed request" );

	std::string extra;
	if (ss >> extra)
		throw SyntaxErrorException( "400 Bad Request: Too many elements in request line" );

	for (size_t i = 0; i < method.size(); i++)
	{
		if (method[i] < 'A' || method[i] > 'Z')
			throw SyntaxErrorException( "400 Bad Request: Method not in uppercase" );
	}

	if (method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD")
		throw SyntaxErrorException( "501 Not Implemented" ); // Erreur peut-être formulée ailleurs
	if (URI.empty() || URI[0] != '/')
		throw SyntaxErrorException( "400 Bad Request: URI must start with '/'" );
	if (protocolVersion != "HTTP/1.1")
		throw SyntaxErrorException( "505 HTTP Version Not Supported" );

	_method = method;
	_URI = URI;
}

void	Request::headerCheck( const std::string& serialized )
{
	size_t headerStart = serialized.find( "\r\n" );
	if (headerStart == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing CRLF after request line" );
	headerStart += 2;

	size_t headerEnd = serialized.find( "\r\n\r\n", headerStart );
	if (headerEnd == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing double CRLF (end of headers)" );

	size_t pos = headerStart;
	while (pos < headerEnd)
	{
		size_t lineEnd = serialized.find( "\r\n", pos );
		if (lineEnd == std::string::npos || lineEnd > headerEnd)
			break ;
		
		std::string line = serialized.substr( pos, lineEnd - pos );
		if (!line.empty())
		{
			std::pair<std::string, std::string> header = _parseHeaderLine( line );

			if (!__isValidHeaderName( header.first ))
				throw SyntaxErrorException( "400 Bad Request: Invalid header name" );
			addHeader( header.first, header.second );
		}
		pos = lineEnd + 2;
	}

	if (!_hasHeader( "Host" ))
		throw SyntaxErrorException( "400 Bad Request: Missing 'Host' header" );
	if (_method == "POST")
	{
		if (!_hasHeader( "Content-Length") && !_hasHeader( "Transfer-Encoding" ))
			throw SyntaxErrorException( "411 Length Required" );
	}
	if (_hasHeader( "Content-Length" ))
	{
		std::string strLen = getHeaderValue( "Content-Length" );
		for (size_t i = 0; i < strLen.size(); i++)
		{
			if (!std::isdigit( static_cast<unsigned char>( strLen[i] ) ))
				throw SyntaxErrorException( "400 Bad Request: Invalid Content-Length" );
		}
	}
}

void	Request::bodyCheck( const std::string& serialized )
{
	//
}

void	Request::deserializeRequest( const std::string& serialized )
{
	requestLineCheck( serialized );
	headerCheck( serialized );
	if (_method == "POST")
		bodyCheck( serialized );
}

/*
	----------------------------- [ Setters ] ----------------------------
*/