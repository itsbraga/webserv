/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:00:39 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/21 01:27:28 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilities.hpp"

std::vector<std::string>	split( const std::string& str, char delimiter )
{
	std::vector<std::string>	tokens;
	std::string					token;
	std::stringstream			ss(str);

	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);

	return (tokens);
}

void	erase_whitespace( std::string& str, const std::string& set )
{
	size_t pos = 0;

	while ((pos = str.find_first_of( set, pos )) != std::string::npos)
		str.erase( pos, 1 );
}