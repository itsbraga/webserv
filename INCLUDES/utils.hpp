/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:35:06 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/03 14:45:30 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <sstream>
# include <string>
# include <vector>

# include "defines.hpp"

// string.cpp
std::vector<std::string>	split( std::string str, char delimiter );

// err_msg.cpp
void	err_msg( const std::string& context, const std::string& reason );
void	err_msg_quoted( const std::string& context, const std::string& reason );

/**************************\
 *	INIT
\**************************/

// init_method_map.cpp
void	init_method_map();

/**************************\
 *	Template functions
\**************************/

template< typename T >
std::string		toString( const T& value )
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}