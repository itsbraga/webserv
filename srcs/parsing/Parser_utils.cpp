/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 12:57:08 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/30 19:10:21 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	--------------------------- [ Utilities ] ----------------------------
*/

std::string	Parser::normalizePath(const std::string& path) const
{
	std::string normalized_path = path;
	
	if (path.size() > 1)
	{
		if (path[path.size() - 1] == '/')
			normalized_path = path.substr(0, path.size() - 1);
	}
	return (normalized_path);
}

bool	Parser::isLeftBrace( const char c ) const
{
	return (c == '{');
}

bool	Parser::isRightBrace( const char c ) const
{
	return (c == '}');
}

bool	Parser::isSemiColon( const char c ) const
{
	return (c == ';');
}

bool	Parser::isWhiteSpace( const char c ) const
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

bool	Parser::isSymbol( const char c ) const
{
	return (c == '{' || c == '}' || c == ';');
}

bool	Parser::isKeyword( const std::string& to_compare ) const
{
	return (_keywords.find( to_compare ) != _keywords.end());
}

bool	Parser::isSymbol( const std::string& to_compare ) const
{
	return (to_compare == "{" || to_compare == "}" || to_compare == ";" );
}

bool	Parser::isValue( const std::string& to_compare ) const
{
	return (isNumber( to_compare ) || isString( to_compare ) \
			|| isPath( to_compare ) || isExtension( to_compare ) \
			|| isStatusCode( to_compare ));
}

bool	Parser::isNumber( const std::string& to_compare ) const
{
	std::string::const_iterator it = to_compare.begin();
	
	for (; it != to_compare.end(); it++)
	{
		if (std::isdigit( static_cast<int>(*it) ) == 0)
			return (false);
	}
	return (true);
}

bool	Parser::isString( const std::string& to_compare ) const
{
	if (to_compare[0] == '.' || to_compare[0] == '-' || to_compare[0] == '_')
		return (false);

	std::string::const_iterator it = to_compare.begin();

	for (; it != to_compare.end(); it++)
	{
		if (std::isalnum( static_cast<int>(*it) ) == 0 && *it != '.' 
			&& *it != '-' && *it != '_')
			return (false);
	}
	return (true);
}

bool	Parser::isPath( const std::string& to_compare ) const
{
	return (to_compare.find( '/' ) != std::string::npos);
}

bool	Parser::isExtension( const std::string& to_compare ) const
{
	return (!to_compare.empty() && to_compare[0] == '.');
}

bool	Parser::isStatusCode( const std::string& to_compare ) const
{
	if (!isNumber( to_compare ) && to_compare.size() != 3)
		return (false);

	unsigned int to_find;
	std::stringstream ss( to_compare );
	ss >> to_find;

	std::vector<unsigned int>::const_iterator it = _status_codes.begin();

	for (; it != _status_codes.end(); ++it)
	{
		if (*it == to_find)
			return (true);
	}
	return (false);
}

bool 	Parser::isErrorStatusCode( const std::string& to_compare ) const
{
	unsigned int code;
	std::stringstream ss( to_compare );
	if (!(ss >> code))
		return (false);

	return (code >= 400 && code <= 599);
}

bool	Parser::isReturnStatusCode( const std::string& to_compare ) const
{
	return (to_compare == "200" || to_compare == "301" || to_compare == "302" \
		|| to_compare == "307" || to_compare == "308" \
		|| to_compare == "403" || to_compare == "404" || to_compare == "410" \
		|| to_compare == "418" || to_compare == "503" );
}

bool	Parser::isValidPort( const std::string& to_check ) const
{
	unsigned int port;
	std::stringstream ss( to_check );
	if (!(ss >> port))
		return (false);

	return (port > 0 && port <= 65535);
}

bool	Parser::isValidMethod( const std::string& to_check ) const
{
	return (to_check == "get" || to_check == "head" || to_check == "post" || to_check == "delete" );
}

bool	Parser::isValidBodySize( const std::string& value ) const
{
	if (value[0] == '-')
		return (false);
	
	size_t i = 0;
	while (i < value.size() && std::isdigit( value[i] ))
		++i;
	
	if (i == 0)
		return (false);
	else if (i == value.size())
		return (true);
	else if (i != value.size() - 1)
		return (false);

	char unit = value[i];
	return (unit == 'k' || unit == 'K' ||
			unit == 'm' || unit == 'M' ||
			unit == 'g' || unit == 'G');
}

bool	Parser::isValidExtension( const std::string& to_compare ) const
{
	return (to_compare == ".py" || to_compare == ".sh" || to_compare == ".php");
}

bool	Parser::isValidPath( const std::string& to_check) const
{
	return (to_check.find("//") == std::string::npos);
}

bool	Parser::isServer( const std::string& to_compare ) const
{
	return (to_compare == "server");
}

bool	Parser::isLocation( const std::string& to_compare ) const
{
	return (to_compare == "location");
}
