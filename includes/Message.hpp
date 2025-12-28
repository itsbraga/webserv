/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:39:18 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/27 20:24:11 by pmateo           ###   ########.fr       */
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

		explicit Message() : _http_version( "HTTP/1.1" ) {}

		std::pair<std::string, std::string>		_parseHeaderLine( const std::string& line ) const;
		bool									_checkProhibitedDuplicate( const std::string& name );

		void				_unchunkBody( const std::string& chunked_data );
	
	public:
		virtual ~Message() {}

		virtual void		process() = 0;

		void				setBody( const std::string& body );

		std::multimap<std::string, std::string>&		getHeaderMap()				{ return (_headers); }

		const std::string&								getHttpVersion() const		{ return (_http_version); }
		const std::string&								getBody() const				{ return (_body); }
		const std::multimap<std::string, std::string>&	getHeaderMap() const		{ return (_headers); }
		const std::string								getHeaderValue( const std::string& key ) const;

		void				addHeader( const std::string& key, const std::string& value );
		void				addHeader( std::pair<std::string, std::string> header );
		bool				hasHeader( const std::string& key ) const;

		const std::string	printHeaderMap() const;
};