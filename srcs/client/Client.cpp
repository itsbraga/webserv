/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:24:31 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/30 22:55:02 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	------------------------ [ Private methods ] -------------------------
*/
bool	Client::_isChunkedComplete( size_t body_start ) const
{
	return (_read_buffer.find( "\r\n0\r\n\r\n", body_start ) != std::string::npos);
}

size_t	Client::_findHeaderInBuffer( const std::string& name, size_t limit ) const
{
	std::string pattern = toLower( name ) + ":";
	size_t pattern_len = pattern.size();

	for (size_t i = 0; i + pattern_len <= limit; ++i)
	{
		if (i > 0 && _read_buffer[i - 1] != '\n')
			continue;
		
		bool match = true;
		for (size_t j = 0; j < pattern_len && match; ++j)
		{
			char lower_buffer = std::tolower( static_cast<unsigned char>( _read_buffer[i + j] ) );
			if (lower_buffer != pattern[j])
				match = false;
		}
		if (match)
			return (i);
	}
	return (std::string::npos);
}

bool	Client::_isContentLengthComplete( size_t header_end, size_t body_start ) const
{
	const std::string header_name = "Content-Length";

	size_t cl_pos = _findHeaderInBuffer( header_name, header_end );
	if (cl_pos == std::string::npos)
		return (true); // Without body

	size_t value_start = cl_pos + header_name.size() + 1;
	size_t cl_end = _read_buffer.find( "\r\n", cl_pos );
	if (cl_end == std::string::npos)
		return (false); // Incomplete header

	std::string cl_value = _read_buffer.substr( value_start, cl_end - value_start );

	size_t content_length = 0;
	if (!parseContentLength( cl_value, content_length ))
		return (true);

	size_t body_received = _read_buffer.size() - body_start;
	return (body_received >= content_length);
}

/*
	------------------------- [ Public methods ] -------------------------
*/
void	Client::appendToReadBuffer( const char *data, size_t len )
{
	_read_buffer.append( data, len );
}

void	Client::appendToWriteBuffer( const std::string& data )
{
	_write_buffer.append( data );
}

bool	Client::hasDataToSend() const
{
	return (!_write_buffer.empty());
}

bool	Client::sendData()
{
	if (_write_buffer.empty())
		return (true);

	// std::cerr << BOLD RED "[DEBUG] fd=" << _socket_fd 
	// 		<< " write_buffer size=" << _write_buffer.size() 
	// 		<< " first 100 chars: " << _write_buffer.substr(0, 100) << NC << std::endl;
	
	ssize_t sent = ::send( _socket_fd, _write_buffer.c_str(), _write_buffer.size(), 0 );
	if (sent > 0)
		_write_buffer.erase( 0, sent );

	return (_write_buffer.empty());
}

void	Client::updateLastActivity()
{
	_last_activity = time( NULL );
}

void	Client::setRequestStartTime()
{
	_request_start = time( NULL );
}

bool	Client::hasCompleteRequest() const
{
	size_t header_end = _read_buffer.find( "\r\n\r\n" );
	if (header_end == std::string::npos)
		return (false);

	size_t body_start = header_end + 4;

	size_t te_pos = _findHeaderInBuffer( "Transfer-Encoding", header_end );
	if (te_pos != std::string::npos)
	{
		size_t te_value_start = te_pos + 18;
		size_t te_end = _read_buffer.find( "\r\n", te_pos );
		if (te_end != std::string::npos && te_end > te_value_start)
		{
			std::string te_value = _read_buffer.substr( te_value_start, te_end - te_value_start );
			std::string lower_te_value = toLower( te_value );
	
			if (lower_te_value.find( "chunked" ) != std::string::npos)
				return (_isChunkedComplete( body_start ));
		}
	}
	return (_isContentLengthComplete( header_end, body_start ));
}

void	Client::clearReadBuffer()
{
	_read_buffer.clear();
}

void	Client::clearWriteBuffer()
{
	_write_buffer.clear();
}

bool	Client::isTimedOut( int timeout ) const
{
	return ((time( NULL ) - _last_activity) > timeout);
}

bool	Client::isRequestTimedOut( int timeout ) const
{
	return ((time(NULL) - _request_start) > timeout);
}
