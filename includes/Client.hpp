/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/28 23:46:40 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Class
\**************************/

class Server;

class Client
{
	private:
		int				_socket_fd;
		std::string		_read_buffer;
		std::string		_write_buffer;
		int				_listener_fd;
		time_t			_last_activity;
		time_t			_request_start;

		bool			_isChunkedComplete( size_t body_start ) const;
		bool			_isContentLengthComplete( size_t header_end, size_t body_start ) const;

		Client();

	public:
		Client( int socket_fd, int listener_fd ) : _socket_fd( socket_fd ), _listener_fd( listener_fd ), _last_activity( time( NULL ) ), _request_start( time(NULL) ) {}
		~Client() {}

		int					getSocketFd() const			{ return (_socket_fd); }
		int					getListenerFd() const		{ return (_listener_fd); }
		const std::string&	getReadBuffer()	const		{ return (_read_buffer); }
		const std::string&	getWriteBuffer() const		{ return (_write_buffer); }
		time_t				getLastActivity() const		{ return (_last_activity); }

		void			appendToReadBuffer( const char *data, size_t len );
		void			appendToWriteBuffer( const std::string& data );
		bool			hasDataToSend() const;
		bool			sendData();
		void			setRequestStartTime();
		void			updateLastActivity();
		bool			hasCompleteRequest() const;
		void			clearReadBuffer();
		void			clearWriteBuffer();
		bool			isTimedOut( int timeout ) const;
		bool			isRequestTimedOut( int timeout ) const;
};