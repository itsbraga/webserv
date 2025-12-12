/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:00:39 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/12 16:33:14 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string>	split( std::string str, char delimiter )
{
	// if (str.empty())
	// 	return (NULL);

	std::vector<std::string>	tokens;
	std::string					token;
	std::stringstream			ss(str);

	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);
	return (tokens);
}

void	erase_whitespace( std::string& str, std::string& set )
{
	std::string::const_iterator set_it = set.begin();
	std::string::iterator 		str_it = str.begin();
	size_t						pos = std::string::npos;

	while (set_it != set.end())
	{
		pos = str.find(*set_it);
		if (pos != std::string::npos)
		{
			str.erase(str_it + pos);
			pos = std::string::npos;
		}
		else
			set_it++;
	}
}