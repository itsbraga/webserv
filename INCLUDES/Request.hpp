/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/10 20:53:17 by annabrag         ###   ########.fr       */
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

		void	_requestLineCheck( const std::string& line );
		void	_headerCheck( const std::string& line );
		void	_bodyCheck( const std::string& line );
		void	_deserializeRequest( const std::string& line );

	public:
		Request( const std::string& serialized );
		~Request() {}

		const std::string&	getMethod() const	{ return (_method); }
		const std::string&	getURI() const		{ return (_URI); }
};