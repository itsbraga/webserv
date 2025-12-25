/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/25 15:16:34 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Class
\**************************/

class Request : public Message
{
	private:
		const std::string	_raw_request;
		std::string			_method;
		std::string			_uri;

		void				_requestLineCheck( const std::string& serialized );

		void				_parseAllHeaders( const std::string& serialized, size_t header_start, size_t header_end );
		void				_validateRequiredHeaders();
		void				_validateContentLength();
		void				_headerCheck( const std::string& serialized );

		void				_handleChunkedBody( const std::string& serialized, size_t body_start );
		void				_handleBody( const std::string& serialized, size_t body_start );
		void				_bodyCheck( const std::string& serialized );

	public:
		Request( const std::string& serialized );
		~Request();

		void				process();

		const std::string&	getMethod() const	{ return (_method); }
		const std::string&	getUri() const		{ return (_uri); }
};