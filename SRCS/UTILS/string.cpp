/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:00:39 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/21 18:36:44 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::vector<std::string>	split(std::string str, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::stringstream ss(str);

	while(std::getline(ss, token, delimiter))
		tokens.push_back(token);
	return (tokens);
}

void	erase_whitespace(std::string& str)
{
	while (str.find("\n\r\t\v") != std::string::npos)
	{
		str.erase(str.find(" \n\r\t\v"));
	}
}