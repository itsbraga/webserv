/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:06:53 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/30 20:03:37 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object manipulation ] -----------------------
*/
ServerConfig::ServerConfig() : _port( 0 ), _auto_index( false ), _return_code( 0 ) {}

ServerConfig::~ServerConfig() {}

ServerConfig&		ServerConfig::operator=( const ServerConfig& toCopy )
{
	if (this != &toCopy)
	{
		_port = toCopy._port;
		_server_name = toCopy._server_name;
		_root = toCopy._root;
		_index = toCopy._index;
		_auto_index = toCopy._auto_index;
		_client_max_body_size = toCopy._client_max_body_size;
		_cgi_extension = toCopy._cgi_extension;
		_return_code = toCopy._return_code;
		_return_uri = toCopy._return_uri; 
		_err_pages = toCopy._err_pages;
		_locations = toCopy._locations;
	}
	return (*this);
}

/*
	---------------------------- [ Setters ] -----------------------------
*/
void	ServerConfig::setPort( unsigned short port )
{
	_port = port;
}

void	ServerConfig::setServerName( const std::string& server_name )
{
	if (server_name.empty())
		return ;

	_server_name = server_name;
}

void	ServerConfig::setRoot( const std::string& root )
{
	if (root.empty())
		return ;

	_root = root;
}

void	ServerConfig::setIndex( const std::string& index )
{
	if (index.empty())
		return ;

	_index = index;
}

void	ServerConfig::setClientMaxSizeBody( const std::string& max_size )
{
	if (max_size.empty())
		return ;

	_client_max_body_size = max_size;
}

void	ServerConfig::setCgiExtension( const std::vector<std::string>& cgi_extension)
{
	if (cgi_extension.empty())
		return ;

	_cgi_extension = cgi_extension;
}

void	ServerConfig::setAutoIndex( bool auto_index )
{
	_auto_index = auto_index;
}

void	ServerConfig::setReturnCode( unsigned int return_code )
{
	_return_code = return_code;
}

void	ServerConfig::setReturnUri( const std::string& return_uri )
{
	if (return_uri.empty())
		return ;

	_return_uri = return_uri;
}

/*
	------------------------- [ Public methods ] -------------------------
*/
std::map<std::string, Location>::const_iterator		ServerConfig::findMatchingLocation( const Request& request ) const
{
	std::string uri = request.getUri();

	size_t pos = uri.find( '?' );
	if (pos != std::string::npos)
		uri = uri.substr( 0, pos );

	std::map<std::string, Location>::const_iterator it = _locations.begin();
	std::map<std::string, Location>::const_iterator current_match = _locations.end();
	size_t current_match_len = 0;
	
	for (; it != _locations.end(); ++it)
	{
		std::string path = it->first;
		size_t path_len = path.size();
		if (path == "/")
		{
			if (path_len > current_match_len)
			{
				current_match = it;
				current_match_len = path_len;
			}
			continue;
		}
		if (uri.find( path ) == 0)
		{
			if (uri.size() == path_len || uri[path_len] == '/')
			{
				if (path_len > current_match_len)
				{
					current_match = it;
					current_match_len = path_len;
				}
			}
		}
	}
	return (current_match);
}

Location	ServerConfig::resolveRoute( const Request& request ) const
{
	std::map<std::string, Location>::const_iterator it = findMatchingLocation( request );

	if (it == _locations.end())
	{
		// std::cout << BOLD RED "Matching location NOT FOUND\n" NC << std::endl;
		Location defaultLoc;

		defaultLoc.setUri( "/" );
		defaultLoc.setServerRoot( _root );
		defaultLoc.setIndex( _index );
		defaultLoc.setErrorPages( _err_pages );
		defaultLoc.setAutoIndex( _auto_index );
		defaultLoc.setClientMaxSizeBody( _client_max_body_size );
		defaultLoc.setUploadAllowed( false );
		return (defaultLoc);
	}

	// std::cout << BOLD P_GREEN "Matching location FOUND!\n" NC << std::endl;
	Location resolved = it->second;

	if (resolved.getRoot().empty())
		resolved.setServerRoot( _root );
	if (resolved.getIndex().empty())
		resolved.setIndex( _index );
	if (resolved.getClientMaxSizeBody().empty())
		resolved.setClientMaxSizeBody( _client_max_body_size );
	if (resolved.getErrorPages().empty())
	{
		resolved.setErrorPages( _err_pages );
		// std::cout << BOLD P_ORANGE "resolved.getErrorPages() " RED "EMPTY ---> server heritage: " NC << std::endl;
		// std::vector<ErrorPage>::const_iterator it = _err_pages.begin();

		// for (; it != _err_pages.end(); ++it)
		// {
		// 	std::cout << " file: " << it->getFile() << " | status: ";
		// 	const std::vector<int>& statuses = it->getStatus();
		// 	for (size_t i = 0; i < statuses.size(); ++i)
		// 	{
		// 		std::cout << statuses[i];
		// 		if (i < statuses.size() - 1)
		// 			std::cout << ", ";
		// 	}
		// 	std::cout << std::endl;
		// }
	}
	// else
	// {
	// 	std::cout << "resolved.getErrorPages(): " << std::endl;
	// 	std::vector<ErrorPage>::const_iterator it = _err_pages.begin();

	// 	for (; it != _err_pages.end(); ++it)
	// 	{
	// 		std::cout << " file: " << it->getFile() << " | status: ";
	// 		const std::vector<int>& statuses = it->getStatus();
	// 		for (size_t i = 0; i < statuses.size(); ++i)
	// 		{
	// 			std::cout << statuses[i];
	// 			if (i < statuses.size() - 1)
	// 				std::cout << ", ";
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// }
	return (resolved);
}

bool	ServerConfig::isMethodAllowed( const Location& location, const std::string& method ) const
{
	const std::vector<std::string>& allowed = location.getAllowedMethods();
	if (allowed.empty())
		return (true);

	std::string lowerMethod = toLower( method );
	return (std::find( allowed.begin(), allowed.end(), lowerMethod ) != allowed.end());
}

/*
	---------------------------- [ Operator ] ----------------------------
*/
std::ostream&	operator<<( std::ostream &os, const ServerConfig& server )
{
	os << RED << "###### [SERVER] ######" << NC << std::endl;

	os << P_BLUE << "PORT -> " << P_YELLOW << server.getPort() << NC << std::endl;
	if (server.getServerName() != "")
		os << P_BLUE << "SERVER_NAME -> " << P_YELLOW << server.getServerName() << NC << std::endl;
	if (server.getRoot() != "")
		os << P_BLUE << "ROOT -> " << P_YELLOW << server.getRoot() << NC << std::endl;
	if (server.getIndex() != "")
		os << P_BLUE << "INDEX -> " << P_YELLOW << server.getIndex() << NC << std::endl;
	if (server.getErrorPages().size() != 0)
	{
		for (size_t i = 0; i < server.getErrorPages().size(); ++i)
		{
			os << P_BLUE << "ERR_PAGE -> " << NC;
			for (size_t j = 0; j < server.getErrorPages()[i].getStatus().size(); ++j)
				os << P_YELLOW << server.getErrorPages()[i].getStatus()[j] << " -> ";
			os << server.getErrorPages()[i].getFile() << NC << std::endl;
		}
	}
	os << P_BLUE << "AUTOINDEX -> " << P_YELLOW;
	if (server.getAutoIndex() == true)
		os << "true" << NC << std::endl;
	else
		os << "false" << NC << std::endl;
	if (server.getClientMaxSizeBody() != "")
		os << P_BLUE << "client_max_body_size -> " << P_YELLOW << server.getClientMaxSizeBody() << NC << std::endl;
	if (server.getReturnCode() != 0)
		os << P_BLUE << "RETURN_CODE -> " << P_YELLOW << server.getReturnCode() << NC << std::endl;
	if (server.getReturnUri() != "")
		os << P_BLUE << "RETURN_URI -> " << P_YELLOW << server.getReturnUri() << NC << std::endl;
	os << P_BLUE << "LOCATION_MAP_SIZE -> " << P_YELLOW << server.getLocations().size() << NC << std::endl;

	return (os);
}