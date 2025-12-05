/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/05 02:45:18 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <string>
# include <fstream>
# include <sstream>
# include <cctype>

# include "Message.hpp"
# include "colors.hpp"

/**************************\
 *	Exception
\**************************/

class SyntaxErrorException : public std::exception
{
	private:
		std::string		_msg;

	public:
		SyntaxErrorException() : _msg( std::string( BOLD RED "Exception caught: " NC ) + "syntax error" ) {}

		explicit SyntaxErrorException( const std::string& msg ) : _msg( std::string( BOLD RED "Exception caught: " NC ) + msg ) {}
		virtual ~SyntaxErrorException() throw() {}
		
		virtual const char	*what() const throw()
		{
			return (_msg.c_str());
		}
};

/**************************\
 *	Class
\**************************/

class Request : public Message
{
	private:
		std::string		_method;
		std::string		_URI;
	
	public:
		// Request( const std::string& serialized );
		Request( const std::string& rawRequest );
		~Request() {}

		void				requestLineCheck( const std::string& serialized );
		void				deserializeRequest( const std::string& serialized );

		const std::string&	getMethod() const	{ return (_method); }
		const std::string&	getURI() const		{ return (_URI); }
};