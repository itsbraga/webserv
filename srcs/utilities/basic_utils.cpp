/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:00:39 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/23 21:27:06 by annabrag         ###   ########.fr       */
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