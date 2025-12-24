/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/24 18:17:24 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Class
\**************************/

class ErrorPage;
class Location;

class Server
{
	private:
		int								_socket;
		sockaddr_in						_addr;
		unsigned short int 				_port;
		std::string 					_server_name;
		std::string						_root;
		std::string						_index;
		bool							_auto_index;
		std::string						_client_max_size_body;
		unsigned int					_return_code;
		std::string						_return_uri;
		std::vector<ErrorPage>			_err_page;
		std::map<std::string, Location>	_locations;

		bool		_createSocket();
		bool		_configureSocket();
		bool		_bindAndListen();
		bool		_setNonBlocking( int fd );
		void		_closeSocket();
		
	public:
		Server();
		~Server();
		Server&		operator=( const Server& toCopy );

		bool		init();
		int			acceptNewClient();

		std::map<std::string, Location>::const_iterator \
		findMatchingLocation( const Request& request ) const;

		void		setPort( unsigned short int port );
		void		setServerName( const std::string& server_name );
		void		setRoot( const std::string& root );
		void		setIndex( const std::string& index );
		void		setClientMaxSizeBody( const std::string& max_size );
		void		setAutoIndex( bool auto_index );
		void		setReturnCode( unsigned int return_code );
		void		setReturnUri( const std::string& return_uri );

		const int&								getSocket() const				{ return (_socket); }
		const sockaddr_in&						getAddress() const				{ return (_addr); }
		const unsigned short int&				getPort() const					{ return (_port); }
		const std::string&						getServerName() const			{ return (_server_name); }
		const std::string&						getRoot() const					{ return (_root); }
		const std::string& 						getIndex() const				{ return (_index); }
		const std::string&						getClientMaxSizeBody() const	{ return (_client_max_size_body); }
		const bool&								getAutoIndex() const			{ return (_auto_index); }
		const unsigned int&						getReturnCode() const			{ return (_return_code); }
		const std::string&						getReturnUri() const			{ return (_return_uri); }
		const std::vector<ErrorPage>&			getErrorPage() const			{ return (_err_page); }
		const std::map<std::string, Location>& 	getLocations() const			{ return (_locations); }

		int&									getSocket()						{ return (_socket); }
		sockaddr_in&							getAddress()					{ return (_addr); }
		unsigned short int&						getPort()						{ return (_port); }
		std::string&							getServerName()					{ return (_server_name); }
		std::string&							getRoot()						{ return (_root); }
		std::string& 							getIndex()						{ return (_index); }
		std::string&							getClientMaxSizeBody()			{ return (_client_max_size_body); }
		bool&									getAutoIndex()					{ return (_auto_index); }
		unsigned int&							getReturnCode()					{ return (_return_code); }
		std::string&							getReturnUri()					{ return (_return_uri); }
		std::vector<ErrorPage>&					getErrorPage()					{ return (_err_page); }
		std::map<std::string, Location>& 		getLocations()					{ return (_locations); }
};