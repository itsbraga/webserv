/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:56:55 by art3mis           #+#    #+#             */
/*   Updated: 2025/07/15 18:29:36 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <iostream>
# include <string>
# include "defines.hpp"
# include "colors.hpp"

/**************************\
 *	Utils
\**************************/

void	err_msg(const std::string &context, const std::string &reason);
void	err_msg_quoted(const std::string &context, const std::string &reason);