/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:24:31 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/11 23:09:45 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::appendToReadBuffer( const char *data, size_t len )
{
	_read_buffer.append( data, len );
}

bool	Client::hasCompleteRequest() const
{
	size_t header_end = _read_buffer.find( "\r\n\r\n" );
	if (header_end == std::string::npos)
		return (false);

	size_t cl_POST = _read_buffer.find( "Content-Length:" );
	if (cl_POST == std::string::npos)
		return (true);	// Pas de body

	size_t cl_end = _read_buffer.find( "\r\n", cl_POST );
	std::string cl_value = _read_buffer.substr( cl_POST + 15, cl_end - cl_POST - 15 );

	size_t start = cl_value.find_first_not_of( " " );
	if (start == std::string::npos)
		return (true);

	int content_length = std::atoi( cl_value.c_str() + start);
	size_t body_start = header_end + 4;
	size_t body_received = _read_buffer.size() - body_start;

	return (body_received >= static_cast<size_t>( content_length ));
}

void	Client::clearReadBuffer()
{
	_read_buffer.clear();
}