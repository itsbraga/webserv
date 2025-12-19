/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/19 04:05:13 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Server
{
	private :
		unsigned short int 		_port;
		std::string 			_server_name;
		std::string				_root;
		std::string				_index;
		std::vector<ErrorPage>	_epage;
		bool					_auto_index;
		std::string				_client_max_size_body;
		unsigned int			_return_code;
		std::string				_return_uri;
		std::map<std::string, Location>	_locations;
		
	public : 
		Server();
		~Server();


		void					setPort( unsigned short int port );
		void					setServerName( const std::string server_name );
		void					setRoot( const std::string root );
		void					setIndex( const std::string index );
		void					setErrorPage( const std::vector<ErrorPage> error_page );
		void					setClientMaxSizeBody( const std::string max_size );
		void					setAutoIndex( const bool auto_index );
		void					setReturnCode( const unsigned int return_code );
		void					setReturnUri( const std::string return_uri );
		unsigned short int							getPort() const;
		std::string									getServerName() const;
		std::string									getRoot() const;
		std::string 								getIndex() const;
		std::vector<ErrorPage>						getErrorPage() const;
		std::string									getClientMaxSizeBody(std::string max_size) const;
		bool										getAutoIndex() const;
		unsigned int								getReturnCode() const;
		std::string									getReturnUri() const;
		std::map<std::string, Location> 			getLocations() const;

		unsigned short int							getPort();
		std::string									getServerName();
		std::string									getRoot();
		std::string 								getIndex();
		std::vector<ErrorPage>&						getErrorPage();
		std::string									getClientMaxSizeBody(std::string max_size);
		bool										getAutoIndex();
		unsigned int								getReturnCode();
		std::string									getReturnUri();
		std::map<std::string, Location>& 			getLocations();
		

};