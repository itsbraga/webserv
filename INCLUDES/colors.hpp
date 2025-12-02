/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 20:00:51 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/02 16:55:06 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Modes
\**************************/

# define NC			"\e[0m"
# define BOLD		"\e[1m"
# define ITAL		"\e[3m"
# define UNDERLINE	"\e[4m"

/**************************\
 *	Standard colors
\**************************/

# define BLACK		"\e[30m"
# define GRAY		"\e[90m"
# define RED		"\e[31m"
# define GREEN		"\e[32m"
# define YELLOW		"\e[33m"
# define SAND		"\e[38;2;231;200;160m"
# define DARK_SAND	"\e[38;2;212;163;115m"
# define ORANGE		"\e[38;5;208m"
# define BLUE		"\e[34m"
# define PURPLE		"\e[35m"
# define HOT_PINK	"\e[38;2;255;105;180m"
# define PINK		"\e[38;2;255;182;193m"
# define CYAN		"\e[36m"
# define WHITE		"\e[37m"

# define L_GRAY		"\e[38;2;173;165;165m"
# define L_GRAY2	"\e[38;2;211;211;211m"

/**************************\
 *	Pastel colors
\**************************/

# define P_GREEN	"\e[38;2;173;235;179m"
# define P_DGREEN	"\e[38;2;152;168;105m"
# define P_BLUE		"\e[38;2;179;235;242m"
# define P_YELLOW	"\e[38;2;255;234;150m"
# define P_PURPLE	"\e[38;2;211;211;255m"
# define P_ORANGE	"\e[38;2;255;178;127m"