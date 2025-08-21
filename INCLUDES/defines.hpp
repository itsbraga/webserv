/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:43 by art3mis           #+#    #+#             */
/*   Updated: 2025/08/21 21:01:11 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Exit status
\**************************/

# define SUCCESS 0
# define FAILURE 1

/**************************\
 *	Parsing
\**************************/

# define IDENTIFIER 0
# define KEYWORD 1
# define NUMBER 2
# define SEMICOLON 3
# define LBRACE 4
# define RBRACE 5

/**************************\
 *	Server
\**************************/

# define SERVERNAME_UNDEFINED ""
# define ANY_INTERFACE 0

/**************************\
 *	Epoll
\**************************/

# define MAX_EVENTS 512

/**************************\
 *	Error
\**************************/

# define ERR_PREFIX "\033[1m\033[31mError: \033[0m"
# define ERR_USAGE "\033[1m\033[38;2;255;234;150mUsage:\033[0m ./webserv /path/to/file.conf"