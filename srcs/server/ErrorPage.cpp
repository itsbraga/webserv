/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:46:39 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/29 20:44:18 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	----------------------------- [ Setter ] -----------------------------
*/
void	ErrorPage::setFile( std::string file )
{
	if (file.empty())
		return ;

	_file = file;
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

static bool		__loadErrorPage( const std::string& root, const std::string& filename, std::string& content )
{
	std::string path = root + filename;

	if (!pathExists( path ) || !isRegularFile( path ) || !isReadable( path ))
		return (false);

	try {
		content = readFileContent( path );
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

	std::cerr << "[DEBUG] ErrorPageHandler: status=" << status_code << std::endl;

	Location route = server.resolveRoute( request );
	std::cerr << "[DEBUG] route resolved, root=" << route.getRoot() << std::endl;

	const std::vector<ErrorPage>& routeErrors = route.getErrorPage();
	std::cerr << "[DEBUG] route error_pages count=" << routeErrors.size() << std::endl;

	const ErrorPage* error_page = __findErrorPage( route.getErrorPage(), status_code );
	if (!error_page)
		error_page = __findErrorPage( server.getErrorPage(), status_code );

	if (!error_page)
		return ;

	std::string body;
	if (__loadErrorPage( route.getRoot(), error_page->getFile(), body ))
		response.setGeneratedContent( body, "text/html; charset=utf-8" );
}
