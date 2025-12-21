/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:12:32 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/21 01:21:25 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilities.hpp"

static std::string	__buildAutoindexHeader( const std::string& URI )
{
	std::string html = "<!DOCTYPE html><html><head><title>Index of " + URI + "</title></head><body>";
	html += "<h1>Index of " + URI + "</h1><ul>";

	if (URI != "/")
		html += "<li><a href=\"../\">../</a></li>";

	return (html);
}

static std::string	__buildEntryLink( const std::string& URI, const std::string& name, bool isDir )
{
	std::string link = URI;

	if (URI[URI.size() - 1] != '/')
		link += "/";
	link += name;

	if (isDir)
		link += "/";

	return ("<li><a href=\"" + link + "\">" + name + "</a></li>");
}

std::string		generateAutoindex( const std::string& path, const std::string& URI )
{
	DIR*			dir = opendir( path.c_str() );
	struct dirent*	entry;

	if (!dir)
		throw std::runtime_error( "Cannot open directory" );

	std::string html = __buildAutoindexHeader( URI );
	while ((entry = readdir( dir )) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue ;

		html += __buildEntryLink( URI, name, isDirectory( path + "/" + name ));
	}
	html += "</ul></body></html>";
	closedir( dir );

	return (html);
}
