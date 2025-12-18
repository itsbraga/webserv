/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/16 02:21:08 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

/*
	---------------------------- [ Setters ] -----------------------------
*/
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

void	Location::setAutoIndex( bool auto_index )
{
	_auto_index = auto_index;
}

void	Location::setUploadAllowed( bool upload_allowed )
{
	_upload_allowed = upload_allowed;
}

void	Location::setReturnCode( unsigned int return_code )
{
	_return_code = return_code;
}

void	Location::setReturnURI( const std::string& return_URI )
{
	if (return_URI.empty())
		return ;

	_return_URI = return_URI;
}

void	Location::setErrorPage( const std::vector<ErrorPage>& error_page )
{
	if (error_page.empty())
		return ;

	_error_page = error_page;
}

void	Location::setAllowedMethods( const std::vector<std::string>& allowed_methods )
{
	if (allowed_methods.empty())
		return ;

	for (size_t i = 0; i < allowed_methods.size(); i++)
	{
		const std::string& m = allowed_methods[i];
		if (m != "GET" && m != "POST" && m != "DELETE" && m != "HEAD")
			return ;
	}
	_allowed_methods = allowed_methods;
}

void	Location::setCgiBin( const std::vector< std::pair<std::string, std::string> >& cgi_bin )
{
	if (cgi_bin.empty())
		return ;

	_cgi_bin = cgi_bin;
}

/*
	---------------------------- [ Getters ] -----------------------------
*/
const std::vector<ErrorPage>&	Location::getErrorPage() const
{
	return (_error_page);
}

const std::vector<std::string>&		Location::getAllowedMethods() const
{
	return (_allowed_methods);
}

const std::vector<std::pair<std::string, std::string> >&	Location::getCgiBin() const
{
	return (_cgi_bin);
}