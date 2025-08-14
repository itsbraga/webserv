/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_epoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:03:26 by art3mis           #+#    #+#             */
/*   Updated: 2025/08/14 17:40:49 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <string>
# include <cstring>
# include <cstdio>

# include <netinet/in.h>
# include <netinet/ip.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/epoll.h>

# define SUCCESS 0
# define FAILURE 1
# define ERR_PREFIX "\033[1m\033[31mError: \033[0m"
# define R "\e[0m"
# define PY "\e[38;2;255;234;150m"
# define PB "\e[38;2;179;235;242m"

/* ************************************************************************** */

static int	__setNonBlocking( int fd )
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		std::cerr << ERR_PREFIX << PY "fcntl(F_GETFL): " R << strerror(errno) << std::endl;
		return (-1);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		std::cerr << ERR_PREFIX << PY "fcntl(F_SETFL | O_NONBLOCK): " R << strerror(errno)
				  << std::endl;
		return (-1);
	}
	return (SUCCESS);
}

static int	__createServerSocket( void )
{
	int			serverSocket; // listeningFd
	sockaddr_in	serverAddr;

	serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == -1)
	{
		std::cerr << ERR_PREFIX << PY "socket(): " R << strerror(errno) << std::endl;
		return (-1);
	}

	std::memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(static_cast< uint16_t >(8080));
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serverSocket, reinterpret_cast< sockaddr* >(&serverAddr), sizeof(serverAddr)) == -1)
	{
		std::cerr << ERR_PREFIX << PY "bind(): " R << strerror(errno) << std::endl;
		::close(serverSocket);
		return (-1);
	}
	if (listen(serverSocket, SOMAXCONN) == -1)
	{
		std::cerr << ERR_PREFIX << PY "listen(): " R << strerror(errno) << std::endl;
		::close(serverSocket);
		return (-1);
	}
	if (__setNonBlocking(serverSocket) == -1)
	{
		::close(serverSocket);
		return (-1);
	}
	return (serverSocket);
}

static int	__createEpollInstance( int serverSocket )
{
	int epollFd = epoll_create(1);
	if (epollFd == -1)
	{
		std::cerr << ERR_PREFIX << PY "epoll_create(): " R << strerror(errno) << std::endl;
		::close(serverSocket);
		return (-1);
	}

	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = serverSocket;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &ev) == -1)
	{
		std::cerr << ERR_PREFIX << PY "epoll_ctl(ADD listen): " R << strerror(errno) << std::endl;
		::close(epollFd), ::close(serverSocket);
		return (-1);
	}
	return (epollFd);
}

static void	__acceptNewClients( int epollFd, int serverSocket )
{
	while (true)
	{
		int clientSocket = ::accept(serverSocket, NULL, NULL);
		if (clientSocket == -1)
		{
			if (errno == EAGAIN)
				break ;
			std::cerr << ERR_PREFIX << PY "accept(): " R << strerror(errno) << std::endl;
			break ;
		}
		if (__setNonBlocking(clientSocket) == -1)
		{
			::close(clientSocket);
			continue ;
		}

		epoll_event client_ev;
		std::memset(&client_ev, 0, sizeof(client_ev));
		client_ev.events = EPOLLIN;
		client_ev.data.fd = clientSocket;
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &client_ev) == -1)
		{
			std::cerr << ERR_PREFIX << PY "epoll_ctl(ADD client): " R << strerror(errno) << std::endl;
			::close(clientSocket);
			continue ;
		}
		std::cout << PB "[INFO] " R << "client accepted" << std::endl; 
	}
}

static bool	__handleServerSocket( int epollFd, epoll_event& event)
{
	if (event.events & EPOLLERR)
	{
		std::cerr << ERR_PREFIX << PY "listen socket error --> STOPPING" R << std::endl;
		return (false);
	}
	if (event.events & EPOLLIN)
		__acceptNewClients(epollFd, event.data.fd);	
	return (true);
}

static void	__handleClientData( int epollFd, int clientSocket )
{
	while (true)
	{
		char buffer[1024];

		ssize_t nBytes = ::recv(clientSocket, buffer, sizeof(buffer), 0);
		if (nBytes > 0)
		{
			std::cout << PY "Message from client: " R;
			std::cout.write(buffer, nBytes), std::cout << std::endl;
			continue ;	
		}
		if (nBytes == 0)
		{
			epoll_ctl(epollFd, EPOLL_CTL_DEL, clientSocket, NULL);
			::close(clientSocket);
			std::cout << PB "[INFO] " R << "client disconnected (EOF)" << std::endl;
			break ;
		}
		break ;
	}
}

static bool	__handleClientSocket( int epollFd, epoll_event& event)
{
	if (event.events & EPOLLERR)
	{
		epoll_ctl(epollFd, EPOLL_CTL_DEL, event.data.fd, NULL);
		::close(event.data.fd);
		std::cout << PB "[INFO] " R << "client closed (EPOLLERR)" << std::endl;
		return (true);
	}
	if (event.events & EPOLLIN)
		__handleClientData(epollFd, event.data.fd);
	return (true);
}

static void	__eventLoop( int epollFd, int serverSocket )
{
	epoll_event events[7]; // remplacer par MAX_EVENTS(512)
	bool running = true;

	while (running)
	{
		int nbReady = epoll_wait(epollFd, events, 7, -1);
		if (nbReady == -1)
		{
			if (errno == EINTR)
				continue ;
			std::cerr << ERR_PREFIX << PY "epoll_wait(): " R << strerror(errno) << std::endl;
			break ;
		}
		for (int i = 0; i < nbReady; i++)
		{
			if (events[i].data.fd == serverSocket)
			{
				if (__handleServerSocket(epollFd, events[i]) == false)
				{
					running = false;
					break ;
				}
			}
			else
				__handleClientSocket(epollFd, events[i]);
		}
	}
}

int	main( void )
{
	int	serverSocket = __createServerSocket();
	if (serverSocket == -1)
		std::exit(FAILURE);

	int epollFd = __createEpollInstance(serverSocket);
	if (epollFd == -1)
		::close(serverSocket), std::exit(FAILURE);

	__eventLoop(epollFd, serverSocket);

	::close(epollFd), ::close(serverSocket);
	return (SUCCESS);
}
