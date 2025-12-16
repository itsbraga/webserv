/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:08:19 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/16 03:30:56 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

# include <string>
# include <exception>

/**************************\
 *	Exceptions
\**************************/

// 4xx : Client error
class BadRequestException : public std::exception
{
	private:
		std::string	_msg;

	public:
		BadRequestException() : _msg( "400 Bad Request" ) {}
		explicit BadRequestException( const std::string& msg ) : _msg( "400 Bad Request: " + msg ) {}
		virtual ~BadRequestException() throw() {}

		virtual const char		*what() const throw()
			{ return (_msg.c_str()); }
};

class ForbiddenException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("403 Forbidden"); }
};

class NotFoundException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("404 Not Found"); }
};

class MethodNotAllowedException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("405 Method Not Allowed"); }
};

class LengthRequiredException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("411 Length Required"); }
};

class PayloadTooLargeException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("413 Payload Too Large"); }
};

class URITooLongException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("414 URI Too Long"); }
};

class TooManyRequestsException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("429 Too Many Requests"); }
};

// 5xx : Server error
class InternalServerErrorException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("500 Internal Server Error"); }
};

class NotImplementedException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("501 Not Implemented"); }
};

class BadGatewayException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("502 Bad Gateway"); }
};

class ServiceUnavailableException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("503 Service Unavailable"); }
};

class GatewayTimeoutException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("504 Gateway Timeout"); }
};

class HTTPVersionNotSupportedException : public std::exception
{
	public:
		virtual const char		*what() const throw()
			{ return ("505 HTTP Version Not Supported"); }
};