/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/30 19:05:39 by annabrag         ###   ########.fr       */
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
		bool			_should_close;

		size_t			_findHeaderInBuffer( const std::string& name, size_t limit ) const;
		bool			_isContentLengthComplete( size_t header_end, size_t body_start ) const;
		bool			_isChunkedComplete( size_t body_start ) const;

		Client();

	public:
		Client( int socket_fd, int listener_fd ) : _socket_fd( socket_fd ), _listener_fd( listener_fd ),
												_last_activity( time( NULL ) ), _request_start( time(NULL) ),
												_should_close( false ) {}
		~Client() {}

		int					getSocketFd() const			{ return (_socket_fd); }
		int					getListenerFd() const		{ return (_listener_fd); }
		const std::string&	getReadBuffer()	const		{ return (_read_buffer); }
		const std::string&	getWriteBuffer() const		{ return (_write_buffer); }
		time_t				getLastActivity() const		{ return (_last_activity); }
		bool				shouldClose() const			{ return (_should_close); }

		void			setShouldClose( bool value )	{ _should_close = value; }
		void			resetForNextRequest()			{ _should_close = false; }

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