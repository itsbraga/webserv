/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:39:18 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/05 02:15:36 by art3mis          ###   ########.fr       */
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
	
	public:
		virtual ~Message() {}

		virtual void		process() = 0;

		void				setHeaderValue( const std::string key, const std::string value );
		void				setBody( const std::string body );

		void				addHeader( const std::string first, const std::string second );

		const std::string&	getHttpVersion() const	{ return (_http_version); }
		const std::string&	getBody() const			{ return (_body); }

		const std::string	getHeaderMap() const;
		const std::string&	getHeaderValue( const std::string& key ) const;
};