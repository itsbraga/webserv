/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:37:59 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/14 01:14:56 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
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
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "keep-alive" );
}

// 201 need a "location" header
void	Response::Created()
{
	_status_code = 201;
	_status_name = "Created";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "keep-alive" );
}

void	Response::Accepted()
{
	_status_code = 202;
	_status_name = "Accepted";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "keep-alive" );
}

void	Response::MovedPermanently()
{
	std::string location = getHeaderValue( "Location" );
	if (location.empty())
		(this->*_builders[500])();

	_status_code = 301;
	_status_name = "Moved Permanently";
	addHeader( "server", "Webserv/1.0" );
	addHeader( "connection", "keep-alive" );
	addHeader( "content-type", "text/html ; charset=utf-8" );

	std::string body = "Moved Permanently. Redirecting to " + location + "\n";

	setBody( body );
	addHeader( "content-length", toString( getBody().length() ) );
}

void	Response::BadRequest()
{
	_status_code = 400;
	_status_name = "Bad Request";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
	addHeader( "content-length", toString( getBody().length() ) );

	// std::string error = "Bad request";
	// std::string message = "Request body could not be read properly.";
}

void	Response::Forbidden()
{
	_status_code = 403;
	_status_name = "Forbidden";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
	addHeader( "content-length", toString( getBody().length() ) );

	// std::string error = "InsufficientPermissions";
	// std::string message = "Insufficient permissions for this request.";
}

void	Response::NotFound()
{
	_status_code = 404;
	_status_name = "Not Found";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );

	std::string body = "<!DOCTYPE html>\n";
	body += "<head>\n	<title>404 Not Found </title>";

	setBody( body );
	addHeader( "content-length", toString( body.length() ));
}

// Allow header depends ??
void	Response::MethodNotAllowed()
{
	_status_code = 405;
	_status_name = "Method Not Allowed";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
	addHeader( "allow", "GET, POST, HEAD, DELETE" );
}

void	Response::LengthRequired()
{
	_status_code = 411;
	_status_name = "Length Required";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
	addHeader( "content-length", toString( getBody().length() ) );

	// std::string error = "LengthRequired";
	// std::string message = "Requests must have a content length header";
}

void	Response::URITooLong()
{
	_status_code = 414;
	_status_name = "URI Too Long";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
	addHeader( "content-length", toString( getBody().length() ) );

	// std::string error = "URI Too Long";
	// std::string message = "The URI provided was too long for the server to process";
}

void	Response::ImATeapot()
{
	_status_code = 418;
	_status_name = "I'm a teapot";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
	addHeader( "content-length", toString( getBody().length() ) );

	// std::string error = "I'mATeapot";
	// std::string message = "Seriously ? Did u just tried to make a gad damn coffee with a fucking teapot ?";
}

// Need a "Retry-After" header 
void	Response::TooManyRequest()
{
	_status_code = 429;
	_status_name = "Too Many Request";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "retry-after", "1800" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
	addHeader( "content-length", toString( getBody().length() ) );

	// std::string error = "TooManyRequest";
	// std::string message = "You're doing that too often ! Are you trying to crash this server ? Try again later.";
}

void	Response::InternalServerError()
{
	_status_code = 500;
	_status_name = "Internal Server Error";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );

	std::string body = "<!DOCTYPE html>\n";
	body += "<head>\n	<title>500 Internal Server Error !</title>";

	setBody( body );
	addHeader( "content-length", toString( getBody().length() ) );
}

void	Response::NotImplemented()
{
	_status_code = 501;
	_status_name = "Not Implemented";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );
}

void	Response::BadGateway()
{
	_status_code = 502;
	_status_name = "Bad Gateway";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );

	std::string body = "<!DOCTYPE html>\n";
	body += "<head>\n	<title>502 Bad Gateway</title>";

	setBody( body );
}

void	Response::ServiceUnavailable()
{
	_status_code = 503;
	_status_name = "Service Unavailable";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );

	std::string body = "<!DOCTYPE html>\n";
	body += "<head>\n	<title>503 Service Unavailable</title>";

	setBody( body );
}

void	Response::GatewayTimeout()
{
	_status_code = 504;
	_status_name = "Gateway Timeout";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );

	std::string body = "<!DOCTYPE html>\n";
	body += "<head>\n	<title>504 Gateway Timeout</title>";

	setBody( body );
}

void	Response::HttpVersionNotSupported()
{
	_status_code = 505;
	_status_name = "HTTP Version Not Supported";
	addHeader( "server", "WEBSERV/1.0" );
	addHeader( "connection", "close" );
	addHeader( "content-type", "text/html ; charset=utf-8" );

	std::string body = "<!DOCTYPE html>\n";
	body += "<head>\n	<title>505 HTTP Version Not Supported</title>";

	setBody( body );
}
