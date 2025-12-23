/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:39:18 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/22 22:06:14 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

		void				_unchunkBody( const std::string& chunked_data );
	
	public:
		virtual ~Message() {}

		virtual void		process() = 0;

		void				addHeader( const std::string& key, const std::string& value );
		void				setHeaderValue( const std::string& key, const std::string& value );
		void				setBody( const std::string& body );

		const std::string&	getHttpVersion() const		{ return (_http_version); }
		const std::string&	getBody() const				{ return (_body); }
		const std::string	getHeaderMap() const;
		const std::string	getHeaderValue( const std::string& key ) const;

};