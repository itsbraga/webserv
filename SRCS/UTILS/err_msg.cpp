/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:04:53 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/12 16:31:58 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void	err_msg( const std::string& context, const std::string& reason )
{
	std::string msg = ERR_PREFIX;

	if (context.empty())
		msg += reason;
	else
		msg += context + ": " + reason;
	std::cerr << msg << std::endl;
}

void	err_msg_quoted( const std::string& context, const std::string& reason )
{
	std::string msg = ERR_PREFIX;

	if (context.empty())
		msg += reason;
	else
		msg += "'" + context + "': " + reason;
	std::cerr << msg << std::endl;
}
