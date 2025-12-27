/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:25:12 by panther           #+#    #+#             */
/*   Updated: 2025/12/27 19:09:21 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

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
	for (size_t i = 0; i < method.size(); ++i)
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
	if (name.empty() == true)
		return (false);
	
	const std::string allowed = "!#$%&'*+-.^_`|~";

	for (size_t i = 0; i < name.size(); ++i)
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

Response*	returnHandler(const Request& request, const ServerConfig& server)
{
	int status_code;
	std::string status_name;
	std::map<std::string, Location>::const_iterator it;
	it = server.findMatchingLocation(request.getUri());
	if (it != server.getLocations().end())
		status_code = it->second.getReturnCode();
	else
		status_code = server.getReturnCode();
	status_name = Response::getStatusNameFromStatusCode(status_code);
	
	Response* response = new Response(status_code, status_name);
	if (status_code == 301 || status_code == 302 || status_code == 307 || status_code == 308)
	{
		std::string return_uri;
		if (it != server.getLocations().end())
			return_uri = it->second.getReturnUri();
		else
			return_uri = server.getReturnUri();
		response->addHeader("location", return_uri);
	}
	return (response);
}

bool	isReturn(const Request& request, const ServerConfig& server)
{
	std::map<std::string, Location>::const_iterator it;
	it = server.findMatchingLocation(request.getUri());
	if (it != server.getLocations().end())
	{
		if (it->second.getReturnCode() != 0)
			return (true);
	}
	else
	{
		if (server.getReturnCode() != 0)
			return (true);
	}
	return (false);
}
