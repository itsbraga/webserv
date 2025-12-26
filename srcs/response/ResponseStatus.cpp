/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:37:59 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/26 23:38:00 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	--------------------- [ Private setter methods ] ---------------------
*/
void	Response::_setCommonHeaders( bool keepAlive )
{
	addHeader( "server", "webserv/1.0" );
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
	--------------------------- [ Successful ] ---------------------------
*/
void	Response::OK()
{
	_status_code = 200;
	_status_name = "OK";
	_setCommonHeaders( true );
}

// 201 need a "location" header
void	Response::Created()
{
	_status_code = 201;
	_status_name = "Created";
	_setCommonHeaders( true );
}

void	Response::Accepted()
{
	_status_code = 202;
	_status_name = "Accepted";
	_setCommonHeaders( true );
}

/*
	--------------------------- [ Redirection ] --------------------------
*/
void	Response::MovedPermanently()
{
	_status_code = 301;
	_status_name = "Moved Permanently";
	_setCommonHeaders( true );
	addHeader( "content-length", "0");
}

void	Response::Found()
{
	_status_code = 302;
	_status_name = "Found";
	_setCommonHeaders( true );
	addHeader( "content-length", "0");
}

/*
	-------------------------- [ Client error ] --------------------------
*/
void	Response::BadRequest()
{
	_status_code = 400;
	_status_name = "Bad Request";
	_setCommonHeaders( false );
	_setErrorPage( "400 Bad Request" );
}

void	Response::Forbidden()
{
	_status_code = 403;
	_status_name = "Forbidden";
	_setCommonHeaders( false );
	_setErrorPage( "403 Forbidden" );
}

void	Response::NotFound()
{
	_status_code = 404;
	_status_name = "Not Found";
	_setCommonHeaders( false );
	_setErrorPage( "404 Not Found" );
}

// Allow header depends ??
void	Response::MethodNotAllowed()
{
	_status_code = 405;
	_status_name = "Method Not Allowed";
	_setCommonHeaders( false );
	addHeader( "allow", "GET, POST, HEAD, DELETE" );
	_setErrorPage( "405 Method Not Allowed" );
}

void	Response::LengthRequired()
{
	_status_code = 411;
	_status_name = "Length Required";
	_setCommonHeaders( false );
	_setErrorPage( "411 Length Required" );
}

void	Response::PayloadTooLarge()
{
	_status_code = 413;
	_status_name = "Payload Too Large";
	_setCommonHeaders( false );
	_setErrorPage( "413 Payload Too Large" );
}

void	Response::URITooLong()
{
	_status_code = 414;
	_status_name = "URI Too Long";
	_setCommonHeaders( false );
	_setErrorPage( "414 URI Too Long" );
}

void	Response::ImATeapot()
{
	_status_code = 418;
	_status_name = "I'm a teapot";
	_setCommonHeaders( false );
	_setErrorPage( "418 I'm a teapot" );
}

// Need a "Retry-After" header 
void	Response::TooManyRequest()
{
	_status_code = 429;
	_status_name = "Too Many Request";
	_setCommonHeaders( false );
	addHeader( "retry-after", "1800" );
	_setErrorPage( "429 Too Many Request" );
}

/*
	-------------------------- [ Server error ] --------------------------
*/
void	Response::InternalServerError()
{
	_status_code = 500;
	_status_name = "Internal Server Error";
	_setCommonHeaders( false );
	_setErrorPage( "500 Internal Server Error" );
}

void	Response::NotImplemented()
{
	_status_code = 501;
	_status_name = "Not Implemented";
	_setCommonHeaders( false );
	_setErrorPage( "501 Not Implemented" );
}

void	Response::BadGateway()
{
	_status_code = 502;
	_status_name = "Bad Gateway";
	_setCommonHeaders( false );
	_setErrorPage( "502 Bad Gateway" );
}

void	Response::ServiceUnavailable()
{
	_status_code = 503;
	_status_name = "Service Unavailable";
	_setCommonHeaders( false );
	_setErrorPage( "503 Service Unavailable" );
}

void	Response::GatewayTimeout()
{
	_status_code = 504;
	_status_name = "Gateway Timeout";
	_setCommonHeaders( false );
	_setErrorPage( "504 Gateway Timeout" );
}

void	Response::HttpVersionNotSupported()
{
	_status_code = 505;
	_status_name = "HTTP Version Not Supported";
	_setCommonHeaders( false );
	_setErrorPage( "505 HTTP Version Not Supported" );
}

std::string	Response::getStatusNameFromStatusCode(int status_code)
{
	switch (status_code)
	{
	case 200: return("OK");
	case 201: return("Created");
	case 202: return("Accepted");
	case 301: return("Moved Permanently");
	case 302: return("Found");
	case 307: return("Temporary Redirect");
	case 308: return("Permanent Redirect");
	case 400: return("Bad Request");
	case 403: return("Forbidden");
	case 404: return("Not Found");
	case 405: return("Method Not Allowed");
	case 410: return("Gone");
	case 411: return("Length Required");
	case 413: return("Payload Too Large");
	case 414: return("URI Too Long");
	case 418: return("Im A Teapot");
	case 429: return("TooManyRequest");
	case 500: return("Internal Server Error");
	case 501: return("Not Implemented");
	case 502: return("Bad Gateway");
	case 503: return("Service Unavailable");
	case 504: return("GatewayTimeout");
	case 505: return("Http Version Not Supported");
	default: return("Unknown");
	}
}
