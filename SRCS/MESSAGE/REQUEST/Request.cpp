/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/23 20:11:53 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Request::Request() : Message() {}

Request::Request( const std::string& serialized ) : Message()
{
	deserializeRequest(serialized);
}

Request::~Request() {}

/*
	----------------------------- [ Parsing ] ----------------------------
*/
static std::string	__rawRequestLineVerif( const std::string& serialized )
{
	size_t eol = serialized.find("\r\n", 0);
	if (eol == std::string::npos)
		throw Request::SyntaxErrorException(); // 400 Bad Request: Missing CRLF
	std::string requestLine = serialized.substr(0, eol);

	if (requestLine.empty() == true)
		throw Request::SyntaxErrorException(); // 400 Bad Request: Empty request line
	if (requestLine[0] == ' ' || requestLine[requestLine.size() - 1] == ' ')
		throw Request::SyntaxErrorException(); // 400 Bad Request: Leading/trailing space
	if (requestLine.find('\t') != std::string::npos)
		throw Request::SyntaxErrorException(); // 400 Bad Request: TAB not allowed
	if (requestLine.find("  ") != std::string::npos)
		throw Request::SyntaxErrorException(); // 400 Bad Request: Double space not allowed

	return (requestLine);
}

void	Request::requestLineCheck( const std::string& serialized )
{
	std::string requestLine = __rawRequestLineVerif(serialized);

	std::stringstream	ss(requestLine);
	std::string			method, URI, protocolVersion;

	if (!(ss >> method >> URI >> protocolVersion))
		throw SyntaxErrorException(); // 400 Bad Request: Malformed request
	std::string extra;
	if (ss >> extra)
		throw SyntaxErrorException(); // 400 Bad Request: Too many elements in request line

	for (size_t i = 0; i < method.size(); i++)
	{
		if (method[i] < 'A' || method[i] > 'Z')
			throw SyntaxErrorException(); // 400 Bad Request: Method not in uppercase
	}
	if (method != "GET" || method != "POST" || method != "DELETE" || method != "HEAD")
		throw SyntaxErrorException(); // 501 Not Implemented
	if (URI.empty() == true || URI[0] != '/')
		throw SyntaxErrorException(); // 400 Bad Request: URI must start with '/'
	if (protocolVersion != "HTTP/1.1")
		throw SyntaxErrorException(); // 505 HTTP Version Not Supported

	this->_method = method;
	this->_URI = URI;
}

void	Request::deserializeRequest( const std::string& serialized )
{
	requestLineCheck(serialized);
	// headerCheck
	if (this->_method == "POST")
		// bodyCheck
}

/*
	----------------------------- [ Setters ] ----------------------------
*/


/*
	----------------------------- [ Getters ] ----------------------------
*/

