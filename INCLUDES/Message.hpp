/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:39:18 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/12 16:39:03 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <string>
# include <vector>
# include <utility>

# include "utils.hpp"
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

class Message
{		 
	protected:
		const std::string									_http_version;
		std::vector< std::pair<std::string, std::string> >	_headers;
		std::string											_body;

		Message() : _http_version( "HTTP/1.1" ) {}

		std::pair<std::string, std::string>		_parseHeaderLine( const std::string& line ) const;
		bool									_hasHeader( const std::string& key ) const;

		void				_unchunkBody( std::string body );
	
	public:
		virtual ~Message() {}

		virtual void		process() = 0;

		void				setHeaderValue( const std::string& key, const std::string& value );
		void				setBody( const std::string& body );
		
		const std::string&	getHttpVersion() const	{ return (_http_version); }
		const std::string&	getBody() const			{ return (_body); }
		const std::string	getHeaderMap() const;
		const std::string&	getHeaderValue( const std::string& key ) const;

		void				addHeader( const std::string& first, const std::string& second );
};