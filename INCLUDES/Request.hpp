/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/03 18:01:27 by annabrag         ###   ########.fr       */
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

/**************************\
 *	Class
\**************************/

class Request : public Message
{
	private:
		std::string	_method;
		std::string	_URI;
	
	public:
		Request( const std::string& serialized );
		~Request() {}

		void	requestLineCheck( const std::string& serialized );
		void	deserializeRequest( const std::string& serialized );

		class SyntaxErrorException : public std::exception
		{
			const char	*what() const throw();
		};
};