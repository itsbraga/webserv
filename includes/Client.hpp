/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/30 20:40:45 by pmateo           ###   ########.fr       */
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

		pid_t			_cgi_pid;
		int				_cgi_pipe;
		std::string		_cgi_output;
		time_t			_cgi_start;
		time_t			_cgi_last_read;
		bool			_wait_for_cgi;

		size_t			_findHeaderInBuffer( const std::string& name, size_t limit ) const;
		bool			_isContentLengthComplete( size_t header_end, size_t body_start ) const;
		bool			_isChunkedComplete( size_t body_start ) const;

		Client();

	public:
		Client( int socket_fd, int listener_fd ) : _socket_fd( socket_fd ), _listener_fd( listener_fd ),
												_last_activity( time( NULL ) ), _request_start( time(NULL) ),
												_should_close( false ), _cgi_pid(-1), _cgi_pipe(-1),
												_cgi_start(0), _cgi_last_read(0),  _wait_for_cgi(false) {}
		~Client() {}

		int					getSocketFd() const			{ return (_socket_fd); }
		int					getListenerFd() const		{ return (_listener_fd); }
		const std::string&	getReadBuffer()	const		{ return (_read_buffer); }
		const std::string&	getWriteBuffer() const		{ return (_write_buffer); }
		time_t				getLastActivity() const		{ return (_last_activity); }
		bool				shouldClose() const			{ return (_should_close); }

		void			setShouldClose( bool value )	{ _should_close = value; }
		void			resetForNextRequest()			{ _should_close = false; }

		void			setCgiPid(pid_t pid) 				{ _cgi_pid = pid; }
		void			setCgiPipe(int pipe) 				{ _cgi_pipe = pipe; }
		void			setCgiOuput(std::string output) 	{ _cgi_output = output; }
		void			setCgiStart(time_t start) 			{ _cgi_start = start; }
		void			setCgiLastRead(time_t last_read)	{ _cgi_last_read = last_read; }
		void			setWaitForCgi(bool wait)			{ _wait_for_cgi = wait; }

		pid_t			getCgiPid() 						{ return (_cgi_pid); }
		int				getCgiPipe()						{ return (_cgi_pipe); }
		std::string&	getCgiOuput() 						{ return (_cgi_output); }
		time_t			getCgiStart() 						{ return (_cgi_start); }
		time_t			getCgiLastRead()					{ return (_cgi_last_read); }
		bool			getWaitForCgi()						{ return (_wait_for_cgi); }



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