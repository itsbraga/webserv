/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:06:53 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/28 20:43:27 by pmateo           ###   ########.fr       */
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
		_err_page = toCopy._err_page;
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

	size_t pos = uri.find('?');
	if (pos != std::string::npos)
		uri = uri.substr(0, pos);

	std::map<std::string, Location>::const_iterator it = _locations.begin();
	std::map<std::string, Location>::const_iterator current_match = _locations.end();
	size_t current_match_length = 0;
	
	for (; it != _locations.end(); ++it)
	{
		std::string path = it->first;
		size_t path_length = path.length();
		if (path == "/")
		{
			if (path_length > current_match_length)
			{
				current_match = it;
				current_match_length = path_length;
			}
			continue ;
		}
		if (uri.find(path) == 0)
		{
			if (uri.length() == path_length || uri[path_length] == '/')
			{
				if (path_length > current_match_length)
				{
					current_match = it;
					current_match_length = path_length;
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
		Location defaultLoc;
		defaultLoc.setUri( "/" );
		defaultLoc.setServerRoot( _root );
		defaultLoc.setIndex( _index );
		defaultLoc.setAutoIndex( _auto_index );
		defaultLoc.setClientMaxSizeBody( _client_max_body_size );
		defaultLoc.setUploadAllowed( false );
		return (defaultLoc);
	}

	Location resolved = it->second;
	if (resolved.getRoot().empty())
		resolved.setServerRoot( _root );
	if (resolved.getIndex().empty())
		resolved.setIndex( _index );
	if (resolved.getClientMaxSizeBody().empty())
		resolved.setClientMaxSizeBody( _client_max_body_size );
	return (resolved);
}

bool	ServerConfig::isMethodAllowed( const Location& location, const std::string& method ) const
{
	const std::vector<std::string>& allowed = location.getAllowedMethods();

	if (allowed.empty())
		return (true);

	return (std::find( allowed.begin(), allowed.end(), method ) != allowed.end());
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
	if (server.getErrorPage().size() != 0)
	{
		for (size_t i = 0; i < server.getErrorPage().size(); ++i)
		{
			os << P_BLUE << "ERR_PAGE -> " << NC;
			for (size_t j = 0; j < server.getErrorPage()[i].getStatus().size(); ++j)
				os << P_YELLOW << server.getErrorPage()[i].getStatus()[j] << " -> ";
			os << server.getErrorPage()[i].getFile() << NC << std::endl;
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