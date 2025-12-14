/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/14 01:59:50 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Request::Request( const std::string& serialized ) : Message(), _raw_request( serialized )
{
	if (serialized.empty())
		return ;

	process();
}

/*
	-------------------- [ Parsing private methods ] ---------------------

	REMPLACER les SyntaxErrorException par les ResponseStatus
*/
void	Request::_requestLineCheck( const std::string& serialized )
{
	std::string request_line = extractRequestLine( serialized );
	std::string method, URI, protocol_version;


	parseRequestLine( request_line, method, URI, protocol_version );
	validateMethod( method );
	validateURI( URI );
	validateProtocolVersion( protocol_version );

	_method = method;
	_URI = URI;
}

void	Request::_parseAllHeaders( const std::string& serialized, size_t header_start, size_t header_end )
{
	size_t pos = header_start;
	while (pos < header_end)
	{
		size_t line_end = serialized.find( "\r\n", pos );
		if (line_end == std::string::npos || line_end > header_end)
			break ;
		
		std::string line = serialized.substr( pos, line_end - pos );
		if (!line.empty())
		{
			std::pair<std::string, std::string> header = _parseHeaderLine( line );

			if (!isValidHeaderName( header.first ))
				throw SyntaxErrorException( "400 Bad Request: Invalid header name" );
			addHeader( header.first, header.second );
		}
		pos = line_end + 2;
	}
}

void	Request::_validateRequiredHeaders()
{
	if (!_hasHeader( "Host" ))
		throw SyntaxErrorException( "400 Bad Request: Missing 'Host' header" ); // changer d'exception
	if (_method == "POST")
	{
		if (!_hasHeader( "Content-Length") && !_hasHeader( "Transfer-Encoding" ))
			throw SyntaxErrorException( "411 Length Required" ); // changer d'exception
	}
}

void	Request::_validateContentLength()
{
	if (!_hasHeader( "Content-Length" ))
		return ;

	std::string cl_value = getHeaderValue( "Content-Length" );

	for (size_t i = 0; i < cl_value.size(); i++)
	{
		if (!std::isdigit( static_cast<unsigned char>( cl_value[i] ) ))
			throw SyntaxErrorException( "400 Bad Request: Invalid Content-Length" );
	}
}

void	Request::_headerCheck( const std::string& serialized )
{
	size_t header_start = 0, header_end = 0;

	findHeaderBoundaries( serialized, header_start, header_end );
	_parseAllHeaders( serialized, header_start, header_end );
	_validateRequiredHeaders();
	_validateContentLength();
}

void	Request::_chunkedBodyCase( const std::string& serialized, size_t body_start )
{
	std::string encoding = getHeaderValue( "Transfer-Encoding" );

	if (encoding.find( "chunked" ) != std::string::npos)
	{
		std::string chunked_data = serialized.substr( body_start );
		_unchunkBody( chunked_data );
	}
}

void	Request::_fullBodyCase( const std::string& serialized, size_t body_start )
{
	std::string cl_value = getHeaderValue( "Content-Length" );

	int content_length = std::atoi( cl_value.c_str() );
	if (content_length > 0)
	{
		if (body_start + static_cast<size_t>( content_length ) > serialized.size())
			throw SyntaxErrorException( "400 Bad Request: Body shorter than Content-Length" );
		
		_body = serialized.substr( body_start, content_length );
	}
}

void	Request::_bodyCheck( const std::string& serialized )
{
	size_t header_end = serialized.find( "\r\n\r\n" );
	if (header_end == std::string::npos)
		return ;

	size_t body_start = header_end + 4;

	if (_hasHeader( "Transfer-Encoding" ))
		_chunkedBodyCase( serialized, body_start );
	else if (_hasHeader( "Content-Length" ))
		_fullBodyCase( serialized, body_start );
}

/*
	------------------------ [ Polymorph function ] ----------------------
*/
void	Request::process()
{
	if (_raw_request.empty())
		return ;

	_requestLineCheck( _raw_request );
	_headerCheck( _raw_request );

	if (_method == "POST")
		_bodyCheck( _raw_request );
}