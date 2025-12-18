/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:34:05 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/18 22:06:13 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilities.hpp"

/*
	A boundary is a unique string that separates the different parts of the
	request body
*/
static std::string	__extractBoundary( const std::string& content_type )
{
	if (content_type.empty())
		return ("");

	size_t pos = content_type.find( "boundary=" );
	if (pos == std::string::npos)
		return ("");

	return ("--" + content_type.substr( pos + 9 ));
}

static bool		__parseMultipart( const std::string& body, const std::string& boundary, std::string& filename, std::string& file_content )
{
	if (body.empty())
		return (false);

	size_t boundary_pos = body.find( boundary );
	if (boundary_pos == std::string::npos)
		return (false);

	// The final boundary is the marker that indicates the end of all parts of the multipart
	if (body.find( boundary + "--" ) == std::string::npos)
		return (false);

	size_t header_end = body.find( "\r\n\r\n" );
	if (header_end == std::string::npos)
		return (false);

	size_t fn_pos = body.find( "filename=\"" );
	if (fn_pos == std::string::npos)
		return (false);

	size_t fn_start = fn_pos + 10;
	size_t fn_end = body.find( "\"", fn_start );
	if (fn_end == std::string::npos)
		return (false);

	filename = body.substr( fn_start, fn_end - fn_start );
	if (filename.empty())
		return (false);

	size_t content_start = header_end + 4;
	size_t content_end = body.rfind( boundary );
	if (content_end == std::string::npos)
		return (false);

	file_content = body.substr( content_start, content_end - content_start - 2 );
	return (true);
}

std::string		handleUpload( const std::string& body, const std::string& content_type, \
								const std::string& upload_dir, const std::string& upload_route )
{
	std::string boundary = __extractBoundary( content_type );
	if (boundary.empty())
		throw BadRequestException();

	std::string filename;
	std::string file_content;
	if (!__parseMultipart( body, boundary, filename, file_content ))
		throw BadRequestException();

	filename = extractBasename( filename );
	if (!isValidFilename( filename ))
		throw BadRequestException();

	if (!fileExists( upload_dir ) || !isDirectory( upload_dir ))
		throw NotFoundException();
	if (!isWritable( upload_dir ))
		throw ForbiddenException();

	std::string path = upload_dir + "/" + filename; // filesystem path
	bool exists = fileExists( path );

	if (!saveFile( path, file_content ))
		throw InternalServerErrorException();

	std::string URI = upload_route + "/" + filename;
	return (exists ? "" : URI);
}
