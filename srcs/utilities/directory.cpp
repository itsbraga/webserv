/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:10:08 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/25 20:09:50 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

bool	isDirectory( const std::string& path )
{
	struct stat buffer;

	if (stat( path.c_str(), &buffer ) != 0)
		return (false);
	return (S_ISDIR( buffer.st_mode ));
}

std::string		getParentDir( const std::string& path )
{
	size_t pos = path.find_last_of( '/' );
	if (pos == std::string::npos)
		return (".");

	return (path.substr( 0, pos ));
}

static std::vector<std::string>		__removeEmptySegments( const std::vector<std::string>& root_vec )
{
	std::vector<std::string> cleaned_vec;

	for (size_t i = 0; i < root_vec.size(); ++i)
	{
		if (!root_vec[i].empty())
			cleaned_vec.push_back( root_vec[i] );
	}

	return (cleaned_vec);
}

bool	isSafePath( const std::string& root, const std::string& requested_path )
{
	std::vector<std::string> root_vec = split( root, '/' );
	std::vector<std::string> requested_path_vec = split( requested_path, '/' );

	std::vector<std::string> real_path = __removeEmptySegments( root_vec );
	size_t root_depth = real_path.size();

	for (size_t i = 0; i < requested_path_vec.size(); ++i)
	{
		const std::string& segment = requested_path_vec[i];
		if (segment.empty() || segment == ".")
			continue ;
		if (segment == "..")
		{
			if (real_path.size() <= root_depth)
				return (false);
			real_path.pop_back();
		}
		else
			real_path.push_back( segment );
	}

	return (true);
}