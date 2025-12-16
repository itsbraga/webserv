/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:24:31 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/16 03:22:49 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
	------------------------ [ Private methods ] -------------------------
*/
bool	Client::_isChunkedComplete( size_t body_start ) const
{
	return (_read_buffer.find( "\r\n0\r\n\r\n", body_start ) != std::string::npos);
}

bool	Client::_isContentLengthComplete( size_t header_end, size_t body_start ) const
{
	size_t cl_pos = _read_buffer.find( "Content-Length:" );
	if (cl_pos == std::string::npos || cl_pos > header_end)
		return (true); // Without body

	size_t cl_end = _read_buffer.find( "\r\n", cl_pos );
	std::string cl_value = _read_buffer.substr( cl_pos + 15, cl_end - cl_pos - 15 );

	size_t start = cl_value.find_first_not_of( " \t" );
	if (start == std::string::npos)
		return (true);

	int content_length = std::atoi( cl_value.c_str() + start);
	if (content_length < 0)
		return (true);

	size_t body_received = _read_buffer.size() - body_start;
	return (body_received >= static_cast<size_t>( content_length ));
}

/*
	------------------------- [ Public methods ] -------------------------
*/
void	Client::appendToReadBuffer( const char *data, size_t len )
{
	_read_buffer.append( data, len );
}

void	Client::updateLastActivity()
{
	_last_activity = time( NULL );
}

bool	Client::hasCompleteRequest() const
{
	size_t header_end = _read_buffer.find( "\r\n\r\n" );
	if (header_end == std::string::npos)
		return (false);

	size_t body_start = header_end + 4;

	size_t te_pos = _read_buffer.find( "Transfer-Encoding:" );
	if (te_pos != std::string::npos && te_pos < header_end)
	{
		size_t te_end = _read_buffer.find( "\r\n", te_pos );
		std::string te_value = _read_buffer.substr( te_pos + 18, te_end - te_pos - 18 );

		if (te_value.find( "chunked" ) != std::string::npos)
			return (_isChunkedComplete( body_start ));
	}

	return (_isContentLengthComplete( header_end, body_start ));
}

void	Client::clearReadBuffer()
{
	_read_buffer.clear();
}

bool	Client::isTimedOut( int timeout ) const
{
	return ((time( NULL ) - _last_activity) > timeout);
}