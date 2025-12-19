/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/19 21:33:28 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"

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
		~Server() {}


		void					setPort( unsigned short int port );
		void					setServerName( const std::string server_name );
		void					setRoot( const std::string root );
		void					setIndex( const std::string index );
		void					setClientMaxSizeBody( const std::string max_size );
		void					setAutoIndex( const bool auto_index );
		void					setReturnCode( const unsigned int return_code );
		void					setReturnUri( const std::string return_uri );
		const unsigned short int&						getPort() const;
		const std::string&								getServerName() const;
		const std::string&								getRoot() const;
		const std::string& 								getIndex() const;
		const std::vector<ErrorPage>&					getErrorPage() const;
		const std::string&								getClientMaxSizeBody() const;
		const bool&										getAutoIndex() const;
		const unsigned int&								getReturnCode() const;
		const std::string&								getReturnUri() const;
		const std::map<std::string, Location>& 			getLocations() const;

		unsigned short int&								getPort();
		std::string&									getServerName();
		std::string&									getRoot();
		std::string& 									getIndex();
		std::vector<ErrorPage>&							getErrorPage();
		std::string&									getClientMaxSizeBody();
		bool&											getAutoIndex();
		unsigned int&									getReturnCode();
		std::string&									getReturnUri();
		std::map<std::string, Location>& 				getLocations();
		

};

inline std::ostream&	operator<<(std::ostream &os, const Server& to_insert)
{
	os << RED << "### [SERVER] ###" << RESET << std::endl;
	os << BLUE << "PORT -> " << YELLOW << to_insert.getPort() << RESET << std::endl;
	if (to_insert.getServerName() != "")
		os << BLUE << "SERVER_NAME -> " << YELLOW << to_insert.getServerName() << RESET << std::endl;
	if (to_insert.getRoot() != "")
		os << BLUE << "ROOT -> " << YELLOW << to_insert.getRoot() << RESET << std::endl;
	if (to_insert.getIndex() != "")
		os << BLUE << "INDEX -> " << YELLOW << to_insert.getIndex() << RESET << std::endl;
	if (to_insert.getErrorPage().size() != 0)
	{
		for (size_t i = 0; i < to_insert.getErrorPage().size(); ++i)
		{
			os << BLUE << "EPAGE -> " << RESET;
			for (size_t j = 0; j < to_insert.getErrorPage()[i].getStatus().size(); ++j)
				os << YELLOW << to_insert.getErrorPage()[i].getStatus()[j] << ' -> ';
			os << to_insert.getErrorPage()[i].getFile() << RESET << std::endl;
		}
	}
	os << BLUE << "AUTOINDEX -> " << YELLOW;
	if (to_insert.getAutoIndex() == true)
		os << "true" << RESET << std::endl;
	else
		os << "false" << RESET << std::endl;
	if (to_insert.getClientMaxSizeBody() != "")
		os << BLUE << "CLIENT_MAX_SIZE_BODY -> " << YELLOW << to_insert.getClientMaxSizeBody() << RESET << std::endl;
	if (to_insert.getReturnCode() != 0)
		os << BLUE << "RETURN_CODE -> " << YELLOW << to_insert.getReturnCode() << RESET << std::endl;
	if (to_insert.getReturnUri() != "")
		os << BLUE << "RETURN_URI -> " << YELLOW << to_insert.getReturnUri() << RESET << std::endl;
	os << BLUE << "LOCATION_MAP_SIZE -> " << YELLOW << to_insert.getLocations().size() << RESET << std::endl;
	return (os);
}