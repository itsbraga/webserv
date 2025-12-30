/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:46:39 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/30 17:25:52 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	----------------------------- [ Setter ] -----------------------------
*/
void	ErrorPage::setFile( std::string file_path )
{
	if (file_path.empty())
		return ;

	_file_path = file_path;
}

/*
	------------------------- [ Public method ] --------------------------
*/
bool	ErrorPage::findStatus( int target ) const
{
	std::vector<int>::const_iterator it = _status.begin();

	for (; it != _status.end(); it++)
	{
		if (*it == target)
			return (true);
	}
	return (false);
}

/*
	---------------------------- [ Helpers ] -----------------------------
*/
static const ErrorPage*	__findErrorPage( const std::vector<ErrorPage>& pages, int status_code )
{
	std::vector<ErrorPage>::const_iterator it = pages.begin();

	for (; it != pages.end(); ++it)
	{
		if (it->findStatus( status_code ))
			return (&(*it));
	}
	return (NULL);
} 

static bool		__loadErrorPage( const std::string& file_path, std::string& content )
{
	if (!pathExists( file_path ) || !isRegularFile( file_path ) || !isReadable( file_path ))
		return (false);

	try {
		content = readFileContent( file_path );
		return (true);
	}
	catch (const std::exception& e) {
		return (false);
	}
}

/*
	------------------------- [ Handler ] --------------------------
*/
void	ErrorPageHandler( Response& response, const Request& request, const ServerConfig& server )
{
	int status_code = response.getStatusCode();
	if (status_code < 400)
		return ;

	Location route = server.resolveRoute( request );

	const ErrorPage* err_page = __findErrorPage( route.getErrorPages(), status_code );
	if (!err_page)
		err_page = __findErrorPage( server.getErrorPages(), status_code );
	if (!err_page)
		return ;

	std::string body;
	if (__loadErrorPage( err_page->getFile(), body ))
		response.replaceBody( body, "text/html; charset=utf-8" );
}
