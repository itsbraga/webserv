/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 23:00:45 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/23 17:30:46 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

bool	pathExists( const std::string& path )
{
	struct stat buffer;

	return (stat( path.c_str(), &buffer ) == 0);
}

std::string		resolvePath( Server& server, const std::string& uri )
{
	if (uri == "/")
		return (server.getRoot() + "/" + server.getIndex());

	return (server.getRoot() + uri);
}

bool	isRegularFile( const std::string& path )
{
	struct stat buffer;

	if (stat( path.c_str(), &buffer ) != 0)
		return (false);

	return (S_ISREG( buffer.st_mode ));
}

bool	isReadable( const std::string& path )
{
	return (access( path.c_str(), R_OK ) == 0);
}

bool	isWritable( const std::string& path )
{
	return (access( path.c_str(), W_OK ) == 0);
}

/*
	std::ios::binary
	Prevents automatic end-of-line conversion
*/
std::string		readFileContent( const std::string& path )
{
	std::ifstream		file( path.c_str(), std::ios::binary );
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

bool	deleteFile( const std::string& path )
{
	return (unlink( path.c_str() ) == 0);
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
