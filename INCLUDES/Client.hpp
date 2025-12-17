/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/17 14:31:17 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

# include <string>
# include <cstdlib>
# include <ctime>

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
		time_t			_last_activity;

		bool			_isChunkedComplete( size_t body_start ) const;
		bool			_isContentLengthComplete( size_t header_end, size_t body_start ) const;

	public:
		Client( int socket, Server* server ) : _socket( socket ), _server( server ), _last_activity( time( NULL ) ) {}
		~Client() {}

		int				getSocket() const			{ return (_socket); }
		Server*			getServer() const			{ return (_server); }
		std::string&	getReadBuffer()				{ return (_read_buffer); }
		std::string&	getWriteBuffer()			{ return (_write_buffer); }
		time_t			getLastActivity() const		{ return (_last_activity); }

		void			appendToReadBuffer( const char *data, size_t len );
		void			updateLastActivity();
		bool			hasCompleteRequest() const;
		void			clearReadBuffer();
		bool			isTimedOut( int timeout ) const;
};