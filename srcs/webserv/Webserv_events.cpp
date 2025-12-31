/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv_events.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 00:09:52 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/31 00:30:57 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	------------------------ [ P: Epoll events ] -------------------------
*/
bool	Webserv::_addServerToEpoll( int server_fd )
{
	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = server_fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev ) == -1)
		return (err_msg( "epoll_ctl(ADD server)", strerror( errno ) ), false);
	else
		return (true);
}

bool	Webserv::_addClientToEpoll( int client_fd )
{
	epoll_event ev;
	std::memset( &ev, 0, sizeof( ev ) );
	ev.events = EPOLLIN;
	ev.data.fd = client_fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, client_fd, &ev ) == -1)
		return (err_msg( "epoll_ctl(ADD client)", strerror( errno ) ), false);
	else
		return (true);
}

void	Webserv::_removeClient( int client_fd )
{
	epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, client_fd, NULL );
	::close( client_fd );

	std::cout << P_BLUE "[INFO] " NC "Client closed (fd=" << client_fd << ")" << std::endl;

	_clients.erase( client_fd );
}

bool	Webserv::_modifyEpollEvents( int fd, unsigned int events )
{
	epoll_event ev;
	std::memset( &ev, 0, sizeof(ev) );
	ev.events = events;
	ev.data.fd = fd;

	if (epoll_ctl( _epoll_fd, EPOLL_CTL_MOD, fd, &ev ) == -1)
		return (err_msg( "epoll_ctl(MODIFY)", strerror( errno ) ), false);
	else
		return (true);
}

/*
	--------------- [ P: Client & Listener events handling ] ---------------
*/
void	Webserv::_handleListenerEvent( Listener& listener )
{
	while (true)
	{
		int client_fd = listener.acceptClient();
		if (client_fd == -1)
			break;
	
		if (!_addClientToEpoll( client_fd ))
		{
			::close( client_fd );
			continue;
		}

		Client new_client( client_fd, listener.socket_fd );
		_clients.insert( std::make_pair( client_fd, new_client ) );
		std::cout << P_BLUE "[INFO] " NC "Client accepted (fd=" << client_fd << ")" << std::endl; 
	}
}

void	Webserv::_handleClientRead( int client_fd )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	Client& client = it->second;
	char buffer[8192];

	ssize_t nBytes = ::recv( client_fd, buffer, sizeof(buffer), 0 );
	if (nBytes <= 0)
		_removeClient( client_fd );
	else
	{
		client.appendToReadBuffer( buffer, nBytes );
		client.updateLastActivity();
		if (client.hasCompleteRequest())
			_processRequest( client_fd );
	}
}

void	Webserv::_handleClientWrite( int client_fd )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	Client& client = it->second;

	if (client.sendData())
	{
		if (client.shouldClose())
		{
			std::cout << P_BLUE "[INFO] " NC "Closing connection as requested..." << std::endl;
			return (_removeClient( client_fd ));
		}
		if (!_modifyEpollEvents( client_fd, EPOLLIN ))
			return ;
		client.clearReadBuffer();
		client.setRequestStartTime();
	}
}

void	Webserv::_handleClientEvent( int client_fd, unsigned int events )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	if (events & (EPOLLERR | EPOLLHUP))
		return (_removeClient( client_fd ));
	if (events & EPOLLIN)
		_handleClientRead( client_fd );
	if (events & EPOLLOUT)
		_handleClientWrite( client_fd );
}

/*
	------------------- [ P: Client timeout cleanup ] --------------------
*/
void	Webserv::_checkClientTimeout()
{
	std::map<int, Client>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		int client_fd = it->first;
		if (it->second.isTimedOut( CLIENT_INACTIVITY_TIMEOUT ))
		{
			++it;
			std::cout << P_BLUE "[INFO] " NC "Client inactive timeout (fd=" << client_fd << ")" << std::endl;
			_removeClient( client_fd );
			continue;
		}
		else if (it->second.isRequestTimedOut( CLIENT_SLOWLORIS_TIMEOUT ))
		{
			++it;
			std::cout << P_BLUE "[INFO] " NC "Client request timeout (fd=" << client_fd << ")" << std::endl;
			_removeClient( client_fd );
			continue;
		}
		else
			++it;
	}
}