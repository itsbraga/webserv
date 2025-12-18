/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/18 17:21:55 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, Response::ResponseFunction>	Response::_builders;
std::map<std::string, std::string>			Response::_content_types;

/*
	---------------------- [ Object manipulation ] -----------------------
*/
Response::Response( const int status_code, const std::string& status_name )
					: Message(), _status_code( status_code ), _status_name( status_name )
{
	if (status_name.empty())
		return ;

	process();
}

/*
	----------------------------- [ Setters ] ----------------------------
*/
void	Response::setStatusCode( const int status_code )
{
	_status_code = status_code;
}

void	Response::setStatusName( const std::string& status_name )
{
	if (status_name.empty())
		return ;

	_status_name = status_name;
}

void	Response::setResourcePath( const std::string& requested_resource_path )
{
	if (requested_resource_path.empty())
		return ;

	_resource_path = requested_resource_path;
}

void	Response::setContentLength( const std::string& length )
{
	addHeader( "content-length", length );
}

void	Response::setContentType( const std::string& type )
{
	addHeader( "content-type", type );
}

void	Response::setDate()
{
	addHeader( "date", getDate() );
}

void	Response::setLocation( const std::string& location )
{
	addHeader( "location", location );
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const std::string	Response::getExtension( const std::string& URI ) const
{
	if (URI.empty())
		throw InternalServerErrorException();

	std::size_t dot_pos = URI.find_last_of( '.'  );
	if (dot_pos == std::string::npos)
		throw InternalServerErrorException();

	std::string extension = URI.substr( dot_pos + 1 );
	if (extension.empty())
		throw InternalServerErrorException();
	return (extension);
}

const std::string	Response::getDate() const
{
	char buffer[30];
	buffer[29] = '\0';
	time_t timestamp;
	time( &timestamp );

	struct tm datetime = *localtime( &timestamp );
	strftime( buffer, 30, "%a, %d %b %G %T GMT", &datetime );
	std::string result = buffer;
	return (result);
}

const std::string	Response::getSerializedHeaders() const
{
	std::string result;
	std::vector< std::pair<std::string, std::string> >::const_iterator it;

	for (it = _headers.begin(); it != _headers.end(); it++)
		result += it->first + ": " + it->second + "\r\n";
	return (result);
}

// Need to check if adding header date here is ok
const std::string	Response::getSerializedResponse()
{
	std::string response;

	addHeader( "date", getDate() );
	response += _http_version + " ";
	response += toString( _status_code ) + " " + _status_name + "\r\n";
	response += getSerializedHeaders() + "\r\n" + getBody() + "\n";
	return (response);
}

void	Response::defineContentType()
{
	std::string extension = getExtension( getResourcePath() );
	if (extension.empty())
		throw InternalServerErrorException();

	std::map<std::string, std::string>::const_iterator it;

	it = _content_types.find( extension );
	if (it != _content_types.end())
		addHeader( "content-type", it->second );
	else
		addHeader( "content-type", "application/octet-stream" );
}

void	Response::loadContent( const std::string& body, const std::string& path )
{
	setResourcePath( path );
	setBody( body );
	setContentLength( toString( body.size() ) );
	defineContentType();
}

/*
	------------------------ [ Polymorph function ] ----------------------
*/
void	Response::process()
{
	try {
		(this->*_builders.at( _status_code ))();
	}
	catch (const std::out_of_range&) {
		(this->*_builders[500])();
	}
}