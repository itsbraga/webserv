/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv_cgi.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 23:41:53 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/31 05:53:36 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	------------------------ [ P: CGI handling ] -------------------------
*/
void 	Webserv::_handleCgiEvent( int pipe_fd, unsigned int events )
{
	std::map<int, int>::iterator it = _cgi_pipes.find( pipe_fd );
	if (it == _cgi_pipes.end())
		return ;

	int client_fd = it->second;

	std::map<int, Client>::iterator client_it = _clients.find( client_fd );
	if (client_it == _clients.end())
	{
		epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, pipe_fd, NULL );
		close( pipe_fd );
		_cgi_pipes.erase( it );
		return ;
	}

	Client& client = client_it->second;

	if (events & (EPOLLERR | EPOLLHUP))
		_handleCgiResponse( client_fd );
	if (events & EPOLLIN)
	{
		char buffer[4096];
		ssize_t bytes = read( pipe_fd, buffer, sizeof( buffer ) );

		if (bytes > 0)
		{
			client.getCgiOuput().append( buffer, bytes );
			client.setCgiLastRead( time(NULL) );
		}
		else if (bytes == 0)
			_handleCgiResponse( client_fd );
		else
			_killCgi( client_fd, 500 );
	}
}

void	Webserv::_handleCgiResponse( int client_fd )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return;

	Client& client = it->second;

	epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, client.getCgiPipe(), NULL );
	close( client.getCgiPipe() );
	_cgi_pipes.erase( client.getCgiPipe() );

	int status;
	waitpid( client.getCgiPid(), &status, 0 );
	removeCgiPid( client.getCgiPid() );

	Response *response = NULL;

	if (WIFEXITED( status ))
	{
		if (WEXITSTATUS( status ) == SUCCESS)
			response = handleOutput( client.getCgiOuput() );
		else if (WEXITSTATUS( status ) == CGI_ERROR)
			response = new Response( 502, "Bad Gateway" );
		else
			response = new Response( 500, "Internal Server Error" );
	}
	else if (WIFSIGNALED( status ))
		response = new Response( 502, "Bad Gateway" );
	else
		response = new Response( 502, "Bad Gateway" );

	// To modify ///////////////////////////////////////////////
	bool close = client.shouldClose();
	if (response->getHeaderValue( "connection" ) == "close")
		close = true;
	client.setShouldClose( close );
	////////////////////////////////////////////////////////////

	std::string serialized = response->getSerializedResponse();
	delete response;
	client.appendToWriteBuffer( serialized );
	client.setResponsePending( true );
	client.setWaitForCgi( false );
	client.getCgiOuput().clear();
	_modifyEpollEvents( client_fd, EPOLLIN | EPOLLOUT );
}

bool 	Webserv::_isCgiPipe( int fd ) const
{
	return (_cgi_pipes.find( fd ) != _cgi_pipes.end());
}

void	Webserv::_killCgi( int client_fd, int status_code )
{
	std::map<int, Client>::iterator it = _clients.find( client_fd );
	if (it == _clients.end())
		return ;

	Client& client = it->second;

	epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, client.getCgiPipe(), NULL );
	close( client.getCgiPipe() );
	_cgi_pipes.erase( client.getCgiPipe() );
	kill( client.getCgiPid(), SIGKILL );
	waitpid( client.getCgiPid(), NULL, 0 );
	removeCgiPid( client.getCgiPid() );

	Response* response = NULL;
	if (status_code == 504)
		response = new Response( 504, "Gateway Timeout" );
	else
		response = new Response( 500, "Internal Server Error" );

	// To modify ///////////////////////////////////////////////
	bool close = client.shouldClose();
	if (response->getHeaderValue( "connection" ) == "close")
		close = true;
	client.setShouldClose( close );
	////////////////////////////////////////////////////////////

	std::string serialized = response->getSerializedResponse();
	delete response;

	client.appendToWriteBuffer( serialized );
	client.setResponsePending( true );
	client.setWaitForCgi( false );
	client.getCgiOuput().clear();
	_modifyEpollEvents( client_fd, EPOLLIN | EPOLLOUT );
}

void	Webserv::_checkCgiTimeout()
{
	time_t tmp = time(NULL);
	std::map<int, Client>::iterator it = _clients.begin();

	for (; it != _clients.end(); ++it)
	{
		Client &client = it->second;

		if (client.getWaitForCgi() == false)
			continue;
		if (tmp - client.getCgiLastRead() > CGI_INACTIVITY_TIMEOUT)
		{
			_killCgi(it->first, 504);
			continue;
		}
		if (tmp - client.getCgiStart() > CGI_SLOWLORIS_TIMEOUT)
		{
			_killCgi(it->first, 504);
			continue;
		}
	}
}

void	Webserv::_killAllUpCgi()
{
	std::map<int, Client>::iterator client_it = _clients.begin();

	for(; client_it != _clients.end(); ++client_it)
	{
		Client& client = client_it->second;

		if (client.getWaitForCgi() == true)
		{
			epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, client.getCgiPipe(), NULL );
			close( client.getCgiPipe() );
			_cgi_pipes.erase( client.getCgiPipe() );
			kill( client.getCgiPid(), SIGKILL );
			waitpid( client.getCgiPid(), NULL, 0 );
			removeCgiPid( client.getCgiPid() );
		}
	}

	std::set<pid_t>::const_iterator it = _up_cgis.begin();

	for (; it != _up_cgis.end(); ++it)
	{
		kill( *it, SIGKILL );
		waitpid( *it, NULL, 0 );
	}
	_up_cgis.clear();
}

void	Webserv::cleanUpForChild()
{
	std::map<int, Client>::iterator client_it = _clients.begin();

	for (; client_it != _clients.end(); ++client_it)
		::close( client_it->first );

	for (size_t i = 0; i < _listeners.size(); ++i)
		_listeners[i].closeSocketFd();

	if (_epoll_fd != -1)
		::close( _epoll_fd );

	std::map<int, int>::iterator it = _cgi_pipes.begin();

	for (; it != _cgi_pipes.end(); ++it)
		close(it->first);
}
