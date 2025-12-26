/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:39:18 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/26 05:16:21 by pmateo           ###   ########.fr       */
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
		std::multimap<std::string, std::string>				_headers;
		std::string											_body;

		Message() : _http_version( "HTTP/1.1" ) {}

		std::pair<std::string, std::string>		_parseHeaderLine( const std::string& line ) const;
		bool									_checkProhibitedDuplicate( const std::string& name );

		void				_unchunkBody( const std::string& chunked_data );
	
	public:
		virtual ~Message() {}

		virtual void		process() = 0;

		void				addHeader( const std::string& key, const std::string& value );
		bool				hasHeader( const std::string& key ) const;
		void				setHeaderValue( const std::string& key, const std::string& value );
		void				setBody( const std::string& body );

		static std::string	__toLower( const std::string& str );

		const std::string&								getHttpVersion() const		{ return (_http_version); }
		const std::string&								getBody() const				{ return (_body); }
		const std::multimap<std::string, std::string>&	getHeaderMap() const		{ return (_headers); }
		const std::string								getHeaderValue( const std::string& key ) const;
		
		std::multimap<std::string, std::string>&		getHeaderMap()				{ return (_headers); }

};