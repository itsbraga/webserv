/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/30 17:08:48 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object manipulation ] -----------------------
*/
Location::Location() : _auto_index( false ), _upload_allowed( false ), _return_code( 0 ) {}

Location::~Location() {}

Location&		Location::operator=( const Location& toCopy )
{
	if (this != &toCopy)
	{
		_uri = toCopy._uri;
		_root = toCopy._root;
		_index = toCopy._index;
		_err_pages = toCopy._err_pages;
		_allowed_methods = toCopy._allowed_methods;
		_cgi_extension = toCopy._cgi_extension;
		_client_max_body_size = toCopy._client_max_body_size;
		_auto_index = toCopy._auto_index;
		_upload_allowed = toCopy._upload_allowed; 
		_upload_path = toCopy._upload_path;
		_return_code = toCopy._return_code;
		_return_uri = toCopy._return_uri;
	}
	return (*this);
}

/*
	---------------------------- [ Setters ] -----------------------------
*/
void	Location::setUri( const std::string& uri )
{
	if (uri.empty())
		return ;

	_uri = uri;
}

void		Location::setServerRoot( const std::string& server_root )
{
	_server_root = server_root;
}


void	Location::setRoot( const std::string& root )
{
	if (root.empty())
		return ;

	_root = root;
}
	
void	Location::setIndex( const std::string& index )
{
	if (index.empty())
		return ;

	_index = index;
}

void	Location::setErrorPages( const std::vector<ErrorPage>& pages )
{
	_err_pages = pages;
}
	
void	Location::setAllowedMethods( const std::vector<std::string>& allowed_methods )
{
	if (allowed_methods.empty())
		return ;

	_allowed_methods = allowed_methods;
}

void	Location::setCgiExtension( const std::vector<std::string>& cgi_extension)
{
	if (cgi_extension.empty())
		return ;

	_cgi_extension = cgi_extension;
}

void	Location::setClientMaxSizeBody( const std::string& max_size )
{
	if (max_size.empty())
		return ;

	_client_max_body_size = max_size;
}

void	Location::setAutoIndex( bool auto_index )
{
	_auto_index = auto_index;
}

void	Location::setUploadAllowed( bool upload_allowed )
{
	_upload_allowed = upload_allowed;
}

void	Location::setUploadPath( const std::string& path )
{
	if (path.empty())
		return ;

	_upload_path = path;
}

void	Location::setReturnCode( unsigned int return_code )
{
	_return_code = return_code;
}

void	Location::setReturnUri( const std::string& return_uri )
{
	if (return_uri.empty())
		return ;

	_return_uri = return_uri;
}
