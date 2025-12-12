/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/12 20:59:51 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <string>
# include <cstdlib>

/**************************\
 *	Class
\**************************/

class Server;

class Client
{
	private:
		int				_socket;
		std::string		_read_buffer;		// donnees recues en attente de parsing
		std::string		_write_buffer;		// reponse en attente d'envoi
		Server*			_server;

		bool			_isChunkedComplete( size_t body_start ) const;
		bool			_isContentLengthComplete( size_t header_end, size_t body_start ) const;

	public:
		Client( int socket, Server* server ) : _socket( socket ), _server( server ) {}
		~Client() {}

		int				getSocket() const	{ return (_socket); }
		Server*			getServer() const	{ return (_server); }
		std::string&	getReadBuffer()		{ return (_read_buffer); }
		std::string&	getWriteBuffer()	{ return (_write_buffer); }

		void			appendToReadBuffer( const char *data, size_t len );
		bool			hasCompleteRequest() const;
		void			clearReadBuffer();
};