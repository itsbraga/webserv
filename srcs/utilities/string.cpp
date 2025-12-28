/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 13:08:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/28 18:51:22 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::vector<std::string>	split( const std::string& str, char delimiter )
{
	std::vector<std::string>	tokens;
	std::string					token;
	std::stringstream			ss(str);

	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);
	return (tokens);
}

std::string		toLower( const std::string& str )
{
	std::string result = str;

	for (size_t i = 0; i < result.size(); ++i)
		result[i] = std::tolower( static_cast<unsigned char>( result[i] ) );
	return (result);
}

void	lowerStr( std::string& str )
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::tolower( static_cast<unsigned char>( str[i] ) );
}

std::string		extractQueryString( const std::string &uri )
{
	std::string result;

	size_t pos = uri.find('?');
	if (pos != std::string::npos)
	{
		result = uri.substr(pos + 1);
		return (result);
	}
	else
		return ("");
}

std::string	decodeUri( const std::string& uri )
{
	std::string result;
	size_t i = 0;

	while (i < uri.length())
	{
		if (uri[i] == '+')
		{
			result += ' ';
			i++;	
		}
		else if (uri[i] == '%' && i + 2 < uri.length())
		{
			std::string hex = uri.substr(i + 1, 2);
			long converted = strtol( hex.c_str(), NULL, 16 );
			char decoded_char = static_cast<char>(converted);
			result += decoded_char;
			i += 3;
		}
		else
		{
			result += uri[i];
			i++;
		}
	}
	return (result);
}

