/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 13:08:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/25 20:09:50 by art3mis          ###   ########.fr       */
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
