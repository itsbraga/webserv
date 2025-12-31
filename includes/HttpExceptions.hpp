/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpExceptions.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:08:19 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/31 01:53:15 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Main class
\**************************/

class HttpException : public std::exception
{
	protected:
		int				_code;
		std::string		_status;
		std::string		_what;

	public:
		HttpException( int code, const std::string& status ) : _code( code ), _status( status ) {}
		HttpException( int code, const std::string& status, const std::string& detail ) : _code( code ),
																						_status( status ),
																						_what( status + ": " + detail ) {}
		virtual ~HttpException() throw() {}

		int					getCode() const			{ return (_code); }
		const std::string&	getStatus() const		{ return (_status); }

		virtual const char	*what() const throw()	{ return (_what.c_str()); }
};

/**************************\
 *	Child classes
\**************************/

// 4xx : Client error
class BadRequestException : public HttpException
{
	public:
		BadRequestException() : HttpException( 400, "Bad Request" ) {}
		BadRequestException( const std::string& detail ) : HttpException( 400, "Bad Request", detail ) {}
};

class ForbiddenException : public HttpException
{
	public:
		ForbiddenException() : HttpException( 403, "Forbidden" ) {}
};

class NotFoundException : public HttpException
{
	public:
		NotFoundException() : HttpException( 404, "Not Found" ) {}
};

class MethodNotAllowedException : public HttpException
{
	public:
		MethodNotAllowedException() : HttpException( 405, "Method Not Allowed" ) {}
};

class GoneException : public HttpException
{
	public:
		GoneException() : HttpException( 410, "Gone" ) {}
};

class LengthRequiredException : public HttpException
{
	public:
		LengthRequiredException() : HttpException( 411, "Length Required" ) {}
};

class PayloadTooLargeException : public HttpException
{
	public:
		PayloadTooLargeException() : HttpException( 413, "Payload Too Large" ) {}
};

class URITooLongException : public HttpException
{
	public:
		URITooLongException() : HttpException( 414, "Request-URI Too Long" ) {}
};

class ImATeapotException : public HttpException
{
	public:
		ImATeapotException() : HttpException( 418, "I'm a teapot" ) {}
};

class TooManyRequestsException : public HttpException
{
	public:
		TooManyRequestsException() : HttpException( 429, "Too Many Requests" ) {}
};

// 5xx : Server error
class InternalServerErrorException : public HttpException
{
	public:
		InternalServerErrorException() : HttpException( 500, "Internal Server Error" ) {}
};

class NotImplementedException : public HttpException
{
	public:
		NotImplementedException() : HttpException( 501, "Not Implemented" ) {}
};

class BadGatewayException : public HttpException
{
	public:
		BadGatewayException() : HttpException( 502, "Bad Gateway" ) {}
};

class ServiceUnavailableException : public HttpException
{
	public:
		ServiceUnavailableException() : HttpException( 503, "Service Unavailable" ) {}
};

class GatewayTimeoutException : public HttpException
{
	public:
		GatewayTimeoutException() : HttpException( 504, "Gateway Timeout" ) {}
};

class HTTPVersionNotSupportedException : public HttpException
{
	public:
		HTTPVersionNotSupportedException() : HttpException( 505, "HTTP Version Not Supported" ) {}
};