/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:37:59 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/15 23:14:55 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
	------------------------ [ Private methods ] -------------------------
*/
void	Response::_setBaseHeaders( bool keepAlive )
{
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", keepAlive ? "keep-alive" : "close" );
}

void	Response::_setErrorPage( const std::string& title )
{
	addHeader( "content-type", "text/html; charset=utf-8" );

	std::string body = "<!DOCTYPE html><head><title>" + title + "</title></head>";
	setBody( body );
	addHeader( "content-length", toString( body.length() ) );
}

/*
	------------------------- [ Public methods ] -------------------------

	For 200, 201, 202 : Body, Content-Length and Content-Type
	need to be set upstream
	Date header need to be set just before the response is send
	to the client

	No content is missing

	For the moment we assume the content-type is html but we
	need to define it. The content length need to be set after
	the body has been recovered.

*/
void	Response::OK()
{
	_status_code = 200;
	_status_name = "OK";
	_setBaseHeaders( true );
}

// 201 need a "location" header
void	Response::Created()
{
	_status_code = 201;
	_status_name = "Created";
	_setBaseHeaders( true );
}

void	Response::Accepted()
{
	_status_code = 202;
	_status_name = "Accepted";
	_setBaseHeaders( true );
}

void	Response::MovedPermanently()
{
	std::string location = getHeaderValue( "Location" );
	if (location.empty())
		(this->*_builders[500])();

	_status_code = 301;
	_status_name = "Moved Permanently";
	_setBaseHeaders( true );
	addHeader( "content-type", "text/html; charset=utf-8" );

	std::string body = "Moved Permanently. Redirecting to " + location + "\n";
	setBody( body );
	addHeader( "content-length", toString( body.length() ) );
}

void	Response::BadRequest()
{
	_status_code = 400;
	_status_name = "Bad Request";
	_setBaseHeaders( false );
	_setErrorPage( "400 Bad Request" );
}

void	Response::Forbidden()
{
	_status_code = 403;
	_status_name = "Forbidden";
	_setBaseHeaders( false );
	_setErrorPage( "403 Forbidden" );
}

void	Response::NotFound()
{
	_status_code = 404;
	_status_name = "Not Found";
	_setBaseHeaders( false );
	_setErrorPage( "404 Not Found" );
}

// Allow header depends ??
void	Response::MethodNotAllowed()
{
	_status_code = 405;
	_status_name = "Method Not Allowed";
	_setBaseHeaders( false );
	addHeader( "allow", "GET, POST, HEAD, DELETE" );
	_setErrorPage( "405 Method Not Allowed" );
}

void	Response::LengthRequired()
{
	_status_code = 411;
	_status_name = "Length Required";
	_setBaseHeaders( false );
	_setErrorPage( "411 Length Required" );
}

void	Response::URITooLong()
{
	_status_code = 414;
	_status_name = "URI Too Long";
	_setBaseHeaders( false );
	_setErrorPage( "414 URI Too Long" );
}

void	Response::ImATeapot()
{
	_status_code = 418;
	_status_name = "I'm a teapot";
	_setBaseHeaders( false );
	_setErrorPage( "418 I'm a teapot" );
}

// Need a "Retry-After" header 
void	Response::TooManyRequest()
{
	_status_code = 429;
	_status_name = "Too Many Request";
	_setBaseHeaders( false );
	addHeader( "retry-after", "1800" );
	_setErrorPage( "429 Too Many Request" );
}

void	Response::InternalServerError()
{
	_status_code = 500;
	_status_name = "Internal Server Error";
	_setBaseHeaders( false );
	_setErrorPage( "500 Internal Server Error" );
}

void	Response::NotImplemented()
{
	_status_code = 501;
	_status_name = "Not Implemented";
	_setBaseHeaders( false );
	_setErrorPage( "501 Not Implemented" );
}

void	Response::BadGateway()
{
	_status_code = 502;
	_status_name = "Bad Gateway";
	_setBaseHeaders( false );
	_setErrorPage( "502 Bad Gateway" );
}

void	Response::ServiceUnavailable()
{
	_status_code = 503;
	_status_name = "Service Unavailable";
	_setBaseHeaders( false );
	_setErrorPage( "503 Service Unavailable" );
}

void	Response::GatewayTimeout()
{
	_status_code = 504;
	_status_name = "Gateway Timeout";
	_setBaseHeaders( false );
	_setErrorPage( "504 Gateway Timeout" );
}

void	Response::HttpVersionNotSupported()
{
	_status_code = 505;
	_status_name = "HTTP Version Not Supported";
	_setBaseHeaders( false );
	_setErrorPage( "505 HTTP Version Not Supported" );
}
