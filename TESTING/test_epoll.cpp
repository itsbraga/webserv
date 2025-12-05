/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_epoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:03:26 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/05 03:17:46 by art3mis          ###   ########.fr       */
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

# define MAX_EVENTS 512

# define ERR_PREFIX "\033[1m\033[31mError: \033[0m"
# define NC "\e[0m"
# define B "\e[1m"
# define PO "\e[38;2;255;178;127m"
# define PY "\e[38;2;255;234;150m"
# define PB "\e[38;2;179;235;242m"

/* ************************************************************************** */

static int	__setNonBlocking( int fd )
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		std::cerr << ERR_PREFIX << PO "fcntl(F_GETFL): " NC << strerror(errno) << std::endl;
		return (-1);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		std::cerr << ERR_PREFIX << PO "fcntl(F_SETFL | O_NONBLOCK): " NC << strerror(errno)
				  << std::endl;
		return (-1);
	}
	return (SUCCESS);
}

static int	__createServerSocket()
{
	int			serverSocket; // listeningFd
	sockaddr_in	serverAddr;

	serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == -1)
	{
		std::cerr << ERR_PREFIX << PO "socket(): " NC << strerror(errno) << std::endl;
		return (-1);
	}

	std::memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(static_cast< uint16_t >(8080));
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associates the socket with a specific IP address and port
	if (bind(serverSocket, reinterpret_cast< sockaddr* >(&serverAddr), sizeof(serverAddr)) == -1)
	{
		std::cerr << ERR_PREFIX << PO "bind(): " NC << strerror(errno) << std::endl;
		::close(serverSocket);
		return (-1);
	}
	// Marks the socket as a passive socket for incoming connection requests (telephone switchboard)
	if (listen(serverSocket, SOMAXCONN) == -1)
	{
		std::cerr << ERR_PREFIX << PO "listen(): " NC << strerror(errno) << std::endl;
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
		std::cerr << ERR_PREFIX << PO "epoll_create(): " NC << strerror(errno) << std::endl;
		::close(serverSocket);
		return (-1);
	}

	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = serverSocket;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &ev) == -1)
	{
		std::cerr << ERR_PREFIX << PO "epoll_ctl(ADD listen): " NC << strerror(errno) << std::endl;
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
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			std::cerr << ERR_PREFIX << PO "accept(): " NC << strerror(errno) << std::endl;
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
			std::cerr << ERR_PREFIX << PO "epoll_ctl(ADD client): " NC << strerror(errno) << std::endl;
			::close(clientSocket);
			continue ;
		}
		std::cout << PB "[INFO] client accepted" NC << std::endl; 
	}
}

static bool	__handleServerSocket( int epollFd, epoll_event& event)
{
	std::cout << PY "[SERVER] Waiting for connections..." NC << std::endl;
	if (event.events & EPOLLERR)
	{
		std::cerr << ERR_PREFIX << PO "listen socket error --> STOPPING" NC << std::endl;
		return (false);
	}
	if (event.events & EPOLLIN)
		__acceptNewClients(epollFd, event.data.fd);	
	return (true);
}

Faire la difference entre une requete complete et incomplete (attendre)
static void	__handleClientData( int epollFd, int clientSocket )
{
	char buffer[8192];
	
	ssize_t nBytes = ::recv(clientSocket, buffer, sizeof(buffer), 0);
	if (nBytes == -1)
		std::cout << ERR_PREFIX << "recv(): fail" << std::endl;
	else if (nBytes == 0)
	{
		epoll_ctl(epollFd, EPOLL_CTL_DEL, clientSocket, NULL);
		::close(clientSocket);
		std::cout << PB "[INFO] " NC << "client disconnected" << std::endl;
		return ;
	}
	else if (nBytes > 0)
		std::cout.write(buffer, nBytes), std::cout << std::endl;

	// For testing
	const char	*response = "Received your request\n";
	::send(clientSocket, response, strlen(response), 0);
}

static bool	__handleClientSocket( int epollFd, epoll_event& event)
{
	if (event.events & EPOLLERR)
	{
		epoll_ctl(epollFd, EPOLL_CTL_DEL, event.data.fd, NULL);
		::close(event.data.fd);
		std::cout << PB "[INFO] client closed (EPOLLERR)" NC << std::endl;
		return (true);
	}
	if (event.events & EPOLLIN)
		__handleClientData(epollFd, event.data.fd);
	return (true);
}

static void	__eventLoop( int epollFd, int serverSocket )
{
	epoll_event events[MAX_EVENTS];
	bool running = true;

	while (running)
	{
		int nbReady = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		if (nbReady == -1)
		{
			if (errno == EINTR)
				continue ;
			std::cerr << ERR_PREFIX << PO "epoll_wait(): " NC << strerror(errno) << std::endl;
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
			__handleClientSocket(epollFd, events[i]);
		}
	}
}

/* ************************************************************************** */

int	main( void )
{
	int	serverSocket = __createServerSocket();
	if (serverSocket == -1)
		std::exit(FAILURE);

	int epollFd = __createEpollInstance(serverSocket);
	if (epollFd == -1)
	{
		::close(serverSocket);
		std::exit(FAILURE);
	}

	__eventLoop(epollFd, serverSocket);

	::close(epollFd), ::close(serverSocket);
	return (SUCCESS);
}
