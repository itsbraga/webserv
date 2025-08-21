/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/21 18:03:48 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::map< int, Response::ResponseFunction >	Response::_builders;
std::map< std::string, std::string >		Response::_content_types;

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Response::Response() : Message(), _status_code(0)
{
	this->initBuilders();
}

Response::Response( const int status_code, const std::string status_name )
					: Message(), _status_code(status_code), _status_name(status_name)
{
	this->initBuilders();
	this->process();
}

Response::~Response() {}

/*
	------------------------ [ Polymorph function ] ----------------------
*/
void	Response::process()
{
	try {
		(this->*_builders.at(this->_status_code))();
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
	this->_status_code = status_code;
}

void	Response::setStatusName( const std::string status_name )
{
	this->_status_name = status_name;
}

void	Response::setRessourcePath( const std::string requested_ressource_path )
{
	this->_ressource_path = requested_ressource_path;
}

void	Response::setContentLength( const std::string length )
{
	this->_headers.push_back(std::make_pair("content-length", length));
}

void	Response::setContentType( const std::string type )
{
	this->_headers.push_back(std::make_pair("content-type", type));
}

void	Response::setDate()
{
	this->_headers.push_back(std::make_pair("date", getDate()));
}

void	Response::setLocation( const std::string location )
{
	this->_headers.push_back(std::make_pair("location", location));
}

/*
	----------------------------- [ Getters ] ----------------------------
*/
const int&	Response::getStatusCode() const
{
	return (this->_status_code);
}

const std::string&	Response::getStatusName() const
{
	return (this->_status_name);
}

const std::string&	Response::getRessourcePath() const
{
	return (this->_ressource_path);
}

const std::string&	Response::getExtension( const std::string& URI ) const
{
	std::size_t dot_pos = URI.find_last_of('.');
	if (dot_pos == std::string::npos)
		throw std::runtime_error("No extension found for " + URI);
	else
	{
		std::string extension = URI.substr(dot_pos + 1);
		return (extension);
	}
}

const std::string&	Response::getDate() const
{
	char buffer[30];
	buffer[29] = '\0';
	time_t timestamp;
	time(&timestamp);

	struct tm datetime = *localtime(&timestamp);
	strftime(buffer, 30, "%a, %d %b %G %T GMT", &datetime);
	std::string result = buffer;
	return (result);
}

const std::string&	Response::getSerializedHeaders() const
{
	std::string result;
	std::vector< std::pair< std::string, std::string > >::const_iterator it;

	for (it = this->_headers.begin(); it != this->_headers.end(); it++)
		result += it->first + ": " + it->second + "\r\n";
	return (result);
}

// Need to check if adding header date here is ok
const std::string&	Response::getSerializedResponse()
{
	std::string response;

	addHeader("date", getDate());
	// response += "HTTP/";
	// response += toString(this->_http_version);
	response += this->_http_version + " ";
	// response += " ";
	response += toString(this->_status_code) + " " + this->_status_name + "\r\n";
	response += getSerializedHeaders() + "\r\n" + getBody() + "\n";
	return (response);
}

void	Response::defineContentType()
{
	std::string extension = getExtension(getRessourcePath());
	if (extension.empty())
		throw InternalServerErrorException();
	else
	{
		std::map< std::string, std::string >::const_iterator it;

		it = this->_content_types.find(extension);
		if (it != this->_content_types.end())
			addHeader("content-type", it->second);
		else
			addHeader("content-type", "application/octet-stream");
	}
}
