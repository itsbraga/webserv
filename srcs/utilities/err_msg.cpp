/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:04:53 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/23 19:59:19 by annabrag         ###   ########.fr       */
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
