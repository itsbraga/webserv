/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/03 19:26:29 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <string>

/**************************\
 *	Class
\**************************/

class Client
{
	private:
		int			_socket;
		std::string	_buffer;
		bool		_requestComplete;

	public:
		Client( int socket ) : _socket(socket), _requestComplete(false) {}
		~Client() {}

		int					getSocket() const	{ return (_socket); }
		std::string&		getBuffer()			{ return (_buffer); }
		const std::string&	getBuffer() const	{ return (_buffer); }

		void	appendToBuffer( const char *data, size_t len );
		void	clearBuffer();
		bool	isRequestComplete() const		{ return (_requestComplete); }
};