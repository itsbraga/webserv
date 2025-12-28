/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/28 03:12:03 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/********************************\
 *	Struct for Virtual Hosting
\********************************/

class ServerConfig;

struct Listener
{
	int							socket_fd;
	sockaddr_in					addr;
	unsigned short				port;
	std::vector<ServerConfig*>	servers;	// First = default

	bool			createSocketFd();
	bool			configureSocket();
	bool			bindAndListen();
	bool			setNonBlocking( int fd );
	bool			init();
	
	void			closeSocketFd();
	int				acceptClient();
	ServerConfig&	resolveVirtualHosting( const std::string& hostname );

	void			printInfo() const;
};

/**************************\
 *	Class
\**************************/

class ErrorPage;
class Location;
class Request;

class ServerConfig
{
	private:
		unsigned short 					_port;
		std::string 					_server_name;
		std::string						_root;
		std::string						_index;
		std::vector<std::string>		_cgi_extension;
		bool							_auto_index;
		std::string						_client_max_body_size;
		unsigned int					_return_code;
		std::string						_return_uri;
		std::vector<ErrorPage>			_err_page;
		std::map<std::string, Location>	_locations;
		
	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig&		operator=( const ServerConfig& toCopy );

		void		setPort( unsigned short port );
		void		setServerName( const std::string& server_name );
		void		setRoot( const std::string& root );
		void		setIndex( const std::string& index );
		void		setClientMaxSizeBody( const std::string& max_size );
		void		setCgiExtension( const std::vector<std::string>& cgi_extension );
		void		setAutoIndex( bool auto_index );
		void		setReturnCode( unsigned int return_code );
		void		setReturnUri( const std::string& return_uri );

		unsigned short&						getPort()						{ return (_port); }
		std::string&						getServerName()					{ return (_server_name); }
		std::string&						getRoot()						{ return (_root); }
		std::string& 						getIndex()						{ return (_index); }
		std::string&						getClientMaxSizeBody()			{ return (_client_max_body_size); }
		std::vector<std::string>&			getCgiExtension()				{ return (_cgi_extension); }
		bool&								getAutoIndex()					{ return (_auto_index); }
		unsigned int&						getReturnCode()					{ return (_return_code); }
		std::string&						getReturnUri()					{ return (_return_uri); }
		std::vector<ErrorPage>&				getErrorPage()					{ return (_err_page); }
		std::map<std::string, Location>& 	getLocations()					{ return (_locations); }

		const unsigned short&					getPort() const					{ return (_port); }
		const std::string&						getServerName() const			{ return (_server_name); }
		const std::string&						getRoot() const					{ return (_root); }
		const std::string& 						getIndex() const				{ return (_index); }
		const std::string&						getClientMaxSizeBody() const	{ return (_client_max_body_size); }
		const std::vector<std::string>&			getCgiExtension() const			{ return (_cgi_extension); }
		const bool&								getAutoIndex() const			{ return (_auto_index); }
		const unsigned int&						getReturnCode() const			{ return (_return_code); }
		const std::string&						getReturnUri() const			{ return (_return_uri); }
		const std::vector<ErrorPage>&			getErrorPage() const			{ return (_err_page); }
		const std::map<std::string, Location>& 	getLocations() const			{ return (_locations); }

		std::map<std::string, Location>::const_iterator		findMatchingLocation( const Request& request ) const;
		Location	resolveRoute( const Request& request ) const;
		bool		isMethodAllowed( const Location& location, const std::string& method ) const;
};