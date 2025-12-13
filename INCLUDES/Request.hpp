/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/13 23:51:34 by panther          ###   ########.fr       */
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
# include <cstdlib>
# include <map>

# include "Message.hpp"
# include "utils.hpp"
# include "colors.hpp"

/**************************\
 *	Class
\**************************/

class Request : public Message
{
	private:
		const std::string&	_raw_request;
		std::string			_method;
		std::string			_URI;

		void			_requestLineCheck( const std::string& serialized );
		void			_parseAllHeaders( const std::string& serialized, size_t header_start, size_t header_end );
		void			_validateRequiredHeaders();
		void			_validateContentLength();
		void			_headerCheck( const std::string& serialized );
		void			_bodyCheck( const std::string& serialized );

	public:
		Request( const std::string& serialized );
		~Request() {}

		void				process();

		const std::string&	getMethod() const	{ return (_method); }
		const std::string&	getURI() const		{ return (_URI); }
};