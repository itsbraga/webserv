/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 23:00:45 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/18 22:07:03 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilities.hpp"

bool	fileExists( const std::string& path )
{
	struct stat buffer;
	return (stat( path.c_str(), &buffer ) == 0);
}

bool	isDirectory( const std::string& path )
{
	struct stat buffer;
	if (stat( path.c_str(), &buffer ) != 0)
		return (false);
	return (S_ISDIR( buffer.st_mode ));
}

bool	isReadable( const std::string& path )
{
	return (access( path.c_str(), R_OK ) == 0);
}

bool	isWritable( const std::string& path )
{
	return (access( path.c_str(), W_OK ) == 0);
}

std::string	readFileContent( const std::string& path )
{
	std::ifstream		file( path.c_str(), std::ios::binary ); // empeche la conversion automatique des fins de ligne
	std::stringstream	buffer;

	if (!file.is_open())
		throw std::runtime_error( "Cannot open file" );
	buffer << file.rdbuf();
	return (buffer.str());
}

bool	saveFile( const std::string& path, const std::string& content )
{
	std::ofstream file( path.c_str(), std::ios::binary );
	if (!file.is_open())
		return (false);

	file.write( content.c_str(), content.size() );
	file.close();
	return (true);
}

std::string		extractBasename( const std::string& filename )
{
	size_t pos = filename.find_last_of( "/\\");
	if (pos == std::string::npos)
		return (filename);

	return (filename.substr( pos + 1));
}

bool	isValidFilename( const std::string& filename )
{
	if (filename.empty())
		return (false);
	if (filename.find( ".." ) != std::string::npos)
		return (false);
	if (filename[0] == '/')
		return (false);
	return (true);
}

static std::string	__buildAutoindexHeader( const std::string& URI )
{
	std::string	html;

	html = "<!DOCTYPE html><html><head><title>Index of " + URI + "</title></head><body>";
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

std::string	generateAutoindex( const std::string& path, const std::string& URI )
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