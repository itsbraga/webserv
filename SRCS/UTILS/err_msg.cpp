/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:04:53 by art3mis           #+#    #+#             */
/*   Updated: 2025/08/23 18:03:32 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	err_msg( const std::string& context, const std::string& reason )
{
	std::string msg = ERR_PREFIX;
	if (context.empty() == false)
		msg += context + ": " + reason;
	else
		msg += reason;
	std::cerr << msg << std::endl;
}

void	err_msg_quoted( const std::string& context, const std::string& reason )
{
	std::string msg = ERR_PREFIX;
	if (context.empty() == false)
		msg += "'" + context + "': " + reason;
	else
		msg += reason;
	std::cerr << msg << std::endl;
}
