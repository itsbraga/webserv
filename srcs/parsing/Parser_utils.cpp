/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 12:57:08 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/25 21:24:07 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	--------------------------- [ Utilities ] ----------------------------
*/
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
	if (to_compare[0] == '.')
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
	if (to_compare.find( '/' ) == std::string::npos)
		return (false);
	
	return (true);
}

bool	Parser::isExtension( const std::string& to_compare ) const
{
	if (to_compare[0] != '.')
		return (false);

	return (true);
}

bool	Parser::isStatusCode( const std::string& to_compare ) const
{
	if (!isNumber( to_compare ) && to_compare.length() != 3)
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
	ss >> code;

	if (code >= 400 && code <= 599)
		return (true);
	
	return (false);
}

bool	Parser::isReturnStatusCode( const std::string& to_compare ) const
{
	if (to_compare == "301" || to_compare == "403" || to_compare == "404" \
		|| to_compare == "418" || to_compare == "503" )
		return (true);
	
	return (false);
}

bool	Parser::isValidPort( const std::string& to_check ) const
{
	unsigned int port;
	std::stringstream ss( to_check );
	ss >> port;

	if (port == 0 || port > 65536)
		return (false);
	
	return (true);
}

bool	Parser::isValidMethod( const std::string& to_check ) const
{
	if (to_check == "get" || to_check == "head" || to_check == "post" || to_check == "delete" )
		return (true);

	return (false);
}

bool	Parser::isValidBodySize( const std::string& value ) const
{
	if (value[0] == '-')
		return (false);
	
	size_t i = 0;
	while (i < value.length() && std::isdigit( value[i] ))
		++i;
	
	if (i == 0)
		return (false);
	else if (i == value.length())
		return (true);
	else if (i != value.length() - 1)
		return (false);

	char unit = value[i];
	return (unit == 'k' || unit == 'K' || unit == 'm' || unit == 'M' || unit == 'g' || unit == 'G');
}

bool	Parser::isValidExtension( const std::string& to_compare ) const
{
	if (to_compare == ".py" || to_compare == ".sh" || to_compare == ".php")
		return (true);

	return (false);
}

bool	Parser::isServer( const std::string& to_compare ) const
{
	return (to_compare == "server");
}

bool	Parser::isLocation( const std::string& to_compare ) const
{
	return (to_compare == "location");
}
