/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/05 20:16:28 by annabrag         ###   ########.fr       */
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
# include <map>

# include "Message.hpp"
# include "colors.hpp"

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
		void				headerCheck( const std::string& serialized );
		void				bodyCheck( const std::string& serialized );
		void				deserializeRequest( const std::string& serialized );

		const std::string&	getMethod() const	{ return (_method); }
		const std::string&	getURI() const		{ return (_URI); }
};