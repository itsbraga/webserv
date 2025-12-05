/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:24:31 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/05 01:01:59 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::appendToReadBuffer( const char *data, size_t len )
{
	_readBuffer.append( data, len );
}

bool	Client::hasCompleteRequest() const
{
	size_t headerEnd = _readBuffer.find( "\r\n\r\n" );
	if (headerEnd == std::string::npos)
		return (false);

	size_t clPOST = _readBuffer.find( "Content-Length:" );
	if (clPOST == std::string::npos)
		return (true);	// Pas de body

	size_t clEnd = _readBuffer.find( "\r\n", clPOST );
	std::string clValue = _readBuffer.substr( clPOST + 15, clEnd - clPOST - 15 );

	size_t start = clValue.find_first_not_of( " " );
	if (start == std::string::npos)
		return (true);

	int contentLength = std::atoi( clValue.c_str() + start);
	size_t bodyStart = headerEnd + 4;
	size_t bodyReceived = _readBuffer.size() - bodyStart;

	return (bodyReceived >= static_cast<size_t>( contentLength ));
}

void	Client::clearReadBuffer()
{
	_readBuffer.clear();
}