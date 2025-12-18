/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:04:53 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/16 02:59:54 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilities.hpp"

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
