/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/10 20:55:36 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Request::Request( const std::string& serialized ) : Message()
{
	if (serialized.empty())
		return ;
	_deserializeRequest( serialized );
}

/*
	----------------------------- [ Helpers ] ----------------------------
*/
static std::string	__rawRequestLineVerif( const std::string& line )
{
	if (line.empty())
		throw SyntaxErrorException( "400 Bad Request: Emtpy string" );

	size_t eol = line.find( "\r\n", 0 );
	if (eol == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing CRLF" );

	std::string requestLine = line.substr( 0, eol );

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

	const std::string allowed = "!#$%&'*+-.^_`|~";

	for (size_t i = 0; i < name.size(); i++)
	{
		char c = name[i];
		if (!std::isalnum( static_cast<unsigned char>( c ) ) && allowed.find( c ) == std::string::npos)
			return (false);
	}
	return (true);
}

/*
	-------------------- [ Parsing private methods ] ---------------------

	REMPLACER les SyntaxErrorException par les ResponseStatus
*/
void	Request::_requestLineCheck( const std::string& line )
{
	try {
		std::string requestLine = __rawRequestLineVerif( line );

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
	catch (const std::exception& e) {
		std::cerr << ERR_PREFIX << e.what() << std::endl;
	}
}

void	Request::_headerCheck( const std::string& line )
{
	size_t headerStart = line.find( "\r\n" );
	if (headerStart == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing CRLF after request line" );
	headerStart += 2;

	size_t headerEnd = line.find( "\r\n\r\n", headerStart );
	if (headerEnd == std::string::npos)
		throw SyntaxErrorException( "400 Bad Request: Missing double CRLF (end of headers)" );

	size_t pos = headerStart;
	while (pos < headerEnd)
	{
		size_t lineEnd = line.find( "\r\n", pos );
		if (lineEnd == std::string::npos || lineEnd > headerEnd)
			break ;
		
		std::string line = line.substr( pos, lineEnd - pos );
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

void	Request::_bodyCheck( const std::string& line )
{
	if (line.empty())
		return ;

	size_t headerEnd = line.find( "\r\n\r\n" );
	if (headerEnd == std::string::npos)
		return ;

	if (_hasHeader( "Transfer-Encoding" )) // chunked
	{
		try {
			_unchunkBody( _body );
		}
		catch (const std::exception& e) {
			std::cerr << ERR_PREFIX << e.what() << std::endl;
		}
	}
	// if (_hasHeader( "Content-Length" ))
		// extraire exactement N octets
}

void	Request::_deserializeRequest( const std::string& line )
{
	_requestLineCheck( line );
	try {
		_headerCheck( line );
	}
	catch (const std::exception& e) {
		std::cerr << ERR_PREFIX << e.what() << std::endl;
	}
	if (_method == "POST")
		_bodyCheck( line );
}
