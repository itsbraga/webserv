/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:12:32 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/23 20:56:39 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static std::string	__buildAutoIndexHeader( const std::string& uri )
{
	std::string html = "<!DOCTYPE html><html><head><title>Index of " + uri + "</title></head><body>";
	html += "<h1>Index of " + uri + "</h1><ul>";

	if (uri != "/")
		html += "<li><a href=\"../\">../</a></li>";

	return (html);
}

static std::string	__buildEntryLink( const std::string& uri, const std::string& name, bool isDir )
{
	std::string link = uri;

	if (uri[uri.size() - 1] != '/')
		link += "/";
	link += name;

	if (isDir)
		link += "/";

	return ("<li><a href=\"" + link + "\">" + name + "</a></li>");
}

std::string		generateAutoIndex( const std::string& path, const std::string& uri )
{
	DIR*			dir = opendir( path.c_str() );
	struct dirent*	entry;

	if (!dir)
		throw std::runtime_error( "Cannot open directory" );

	std::string html = __buildAutoIndexHeader( uri );
	while ((entry = readdir( dir )) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue ;

		html += __buildEntryLink( uri, name, isDirectory( path + "/" + name ));
	}
	html += "</ul></body></html>";
	closedir( dir );

	return (html);
}
