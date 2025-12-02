/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_ipv4.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:39:47 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 16:51:06 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>

int	main( void )
{
	int				gai_ret = 0;
	struct addrinfo	hints;
	struct addrinfo	*result;
	std::string		str_ip = "127.0.0.1";

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	gai_ret = getaddrinfo(str_ip.c_str(), NULL, &hints, &result);
	if (gai_ret != 0)
	{
		std::cerr << "getaddrinfo : " << gai_strerror(gai_ret) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	int sock_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_fd == -1)
	{
		std::cerr << "Socket error : " << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0)
	{
		std::cerr << "Bind error" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::cout << "SUCCESS" << std::endl;
	struct sockaddr_in* addrin = (struct sockaddr_in *)result->ai_addr;
	std::cout << "str_ip serialized is : " << addrin->sin_addr.s_addr << std::endl;
	std::exit(EXIT_SUCCESS);
}