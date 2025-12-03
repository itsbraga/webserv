/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:19:17 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/03 19:30:15 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Webserv::~Webserv()
{
	if (_epollFd != -1)
		::close(_epollFd);

	// Fermer tous les clients
	// std::map<int, Client>::iterator	it = _clients.begin();
	// for ( ; it != _clients.end(); ++it)
	// 	::close(it->first);

	// Supprimer les serveurs
	// for (size_t i = 0; i < _servers.size(); i++)
	// 	delete _servers[i];
}

/*
	------------------------ [ Private methods ] -------------------------
*/
void	Webserv::_handleServerEvent( int serverFd )
{
	Server* server = NULL;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		if (_servers[i]->getSocket() == serverFd)
		{
			server = _servers[i];
			break ;
		}
	}
	if (server == NULL)
		return ;

	while (true)
	{
		int clientFd = server->acceptNewClient();
		if (clientFd == -1)
			break ;

		epoll_event ev;
		std::memset(&ev, 0, sizeof(ev));
		ev.events = EPOLLIN;
		ev.data.fd = clientFd;

		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientFd, &ev) == -1)
		{
			std::cerr << ERR_PREFIX << P_ORANGE "epoll_ctl(ADD client): " NC << strerror(errno) << std::endl;
			::close(clientFd);
			continue ;
		}
		_clients[clientFd] = Client(clientFd);
		std::cout << P_BLUE "[INFO] " NC "Client accepted (fd=" << clientFd << ")" << std::endl; 
	}
	return ;
}

void	Webserv::_handleClientEvent( int clientFd, uint32_t events )
{
	if (events & EPOLLERR)
	{
		epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd, NULL);
		::close(clientFd);
		_clients.erase(clientFd);
		std::cout << P_BLUE "[INFO] " NC "Client closed (EPOLLERR)" << std::endl;
		return ;
	}
	if (events & EPOLLIN)
	{
		char buffer[8192];
	
		ssize_t nBytes = ::recv(clientFd, buffer, sizeof(buffer), 0);
		if (nBytes == -1)
			std::cout << ERR_PREFIX << "recv(): fail" << std::endl;
		if (nBytes == 0)
		{
			epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd, NULL);
			::close(clientFd);
			std::cout << P_BLUE "[INFO] " NC "Client disconnected" << std::endl;
			return ;
		}
		_clients[clientFd].appendToBuffer(buffer, nBytes);
		std::cout.write(buffer, nBytes);
		std::cout << std::endl;

		if (_clients[clientFd].isRequestComplete())
		{
			// For testing
			const char	*response = "Received your request\n";
			::send(clientFd, response, strlen(response), 0);
			_clients[clientFd].clearBuffer();
		}
	}
}

/*
	------------------------- [ Public methods ] -------------------------
*/
int		Webserv::addServer( uint16_t port, const std::string& server_name )
{
	Server* server = new Server(port, server_name);
	if (server->init() == FAILURE)
	{
		delete server;
		return (FAILURE);
	}
	_servers.push_back(server);
	_serverFds.insert(server->getSocket());
	return (SUCCESS);
}

int		Webserv::init()
{
	_epollFd = epoll_create(1);
	if (_epollFd == -1)
	{
		std::cerr << ERR_PREFIX << P_ORANGE "epoll_create(): " NC << strerror(errno) << std::endl;
		return (FAILURE);
	}

	for (size_t i = 0; i < _servers.size(); i++)
	{	
		epoll_event ev;
		std::memset(&ev, 0, sizeof(ev));
		ev.events = EPOLLIN;
		ev.data.fd = _servers[i]->getSocket();

		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
		{
			std::cerr << ERR_PREFIX << P_ORANGE "epoll_ctl(ADD server): " NC << strerror(errno) << std::endl;
			return (FAILURE);
		}
	}
	return (SUCCESS);
}