/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/05 02:43:55 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, Response::ResponseFunction>	Response::_builders;
std::map<std::string, std::string>			Response::_content_types;

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Response::Response( const int status_code, const std::string status_name )
					: Message(), _status_code( status_code ), _status_name( status_name )
{
	this->initBuilders();
	this->process();
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

/*
	----------------------------- [ Setters ] ----------------------------
*/
void	Response::setStatusCode( const int status_code )
{
	_status_code = status_code;
}

void	Response::setStatusName( const std::string status_name )
{
	_status_name = status_name;
}

void	Response::setRessourcePath( const std::string requested_ressource_path )
{
	_ressource_path = requested_ressource_path;
}

void	Response::setContentLength( const std::string length )
{
	_headers.push_back( std::make_pair( "content-length", length ) );
}

void	Response::setContentType( const std::string type )
{
	_headers.push_back( std::make_pair( "content-type", type ) );
}

void	Response::setDate()
{
	_headers.push_back( std::make_pair( "date", getDate() ) );
}

void	Response::setLocation( const std::string location )
{
	_headers.push_back( std::make_pair( "location", location ) );
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const std::string	Response::getExtension( const std::string& URI ) const
{
	std::size_t dot_pos = URI.find_last_of( '.'  );
	if (dot_pos == std::string::npos)
		throw std::runtime_error( "No extension found for " + URI );
	else
	{
		std::string extension = URI.substr( dot_pos + 1 );
		return (extension);
	}
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
	// response += "HTTP/";
	// response += toString(_http_version);
	response += _http_version + " ";
	// response += " ";
	response += toString( _status_code ) + " " + _status_name + "\r\n";
	response += getSerializedHeaders() + "\r\n" + getBody() + "\n";
	return (response);
}

void	Response::defineContentType()
{
	std::string extension = getExtension( getRessourcePath() );
	if (extension.empty())
		throw InternalServerErrorException();
	else
	{
		std::map<std::string, std::string>::const_iterator it;

		it = _content_types.find( extension );
		if (it != _content_types.end())
			addHeader( "content-type", it->second );
		else
			addHeader( "content-type", "application/octet-stream" );
	}
}

/*
	--------------------------- [ Exceptions ] ---------------------------
*/
const char*		Response::RessourceForbiddenException::what() const throw()
{
	return (BOLD RED "Exception caught: " NC "Ressource forbidden");
}

const char*		Response::RessourceNotFoundException::what() const throw()
{
	return (BOLD RED "Exception caught: " NC "Ressource not found");
}

// Ajuster le detail de l'exception
const char*		Response::InternalServerErrorException::what() const throw()
{
	return (BOLD RED "Exception caught: " NC "Internal server error: ???");
}