/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:24:31 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/03 19:26:12 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::appendToBuffer( const char *data, size_t len )
{
	_buffer.append(data, len);
	if (_buffer.find("\r\n\r\n") != std::string::npos)
		_requestComplete = true;
}

void	Client::clearBuffer()
{
	_buffer.clear();
	_requestComplete = false;
}