/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:48:58 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/30 19:52:24 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

bool	parseContentLength( const std::string& value, size_t& out )
{
	if (value.empty())
		return (false);

	size_t start = value.find_first_not_of( " \t" );
	if (start == std::string::npos)
		return (false);

	errno = 0;
	char *endptr;
	unsigned long long parsed = std::strtoull( value.c_str() + start, &endptr, 10 );

	while (*endptr && std::isspace( static_cast<unsigned char>( *endptr ) ))
		++endptr;

	if (*endptr != '\0' || errno == ERANGE)
		return (false);

	out = static_cast<size_t>( parsed );
	return (true);
}