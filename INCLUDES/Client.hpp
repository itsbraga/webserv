/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/05 02:07:30 by art3mis          ###   ########.fr       */
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
		std::string		_readBuffer;		// donnees recues en attente de parsing
		std::string		_writeBuffer;		// reponse en attente d'envoi
		Server*			_server;

	public:
		Client( int socket, Server* server ) : _socket( socket ), _server( server ) {}
		~Client() {}

		int				getSocket() const	{ return (_socket); }
		Server*			getServer() const	{ return (_server); }
		std::string&	getReadBuffer()		{ return (_readBuffer); }
		std::string&	getWriteBuffer()	{ return (_writeBuffer); }

		void			appendToReadBuffer( const char *data, size_t len );
		bool			hasCompleteRequest() const;
		void			clearReadBuffer();
};