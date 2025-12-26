/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:00:39 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/25 18:56:46 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

void	err_msg( const char *context, const std::string& reason )
{
	std::string msg = ERR_PREFIX;

	if (context == NULL || context[0] == '\0')
		msg += reason;
	else
		msg += BOLD P_ORANGE + std::string( context ) + ": " + NC + reason;
	std::cerr << msg << std::endl;
}

void	err_msg_quoted( const char *context, const std::string& reason )
{
	std::string msg = ERR_PREFIX;

	if (context == NULL || context[0] == '\0')
		msg += reason;
	else
		msg += "'" + std::string( context ) + "': " + reason;
	std::cerr << msg << std::endl;
}

std::vector<std::string>	split( const std::string& str, char delimiter )
{
	std::vector<std::string>	tokens;
	std::string					token;
	std::stringstream			ss(str);

	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);

	return (tokens);
}

std::string	extractQueryString( const std::string &uri )
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