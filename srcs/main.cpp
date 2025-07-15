/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/07/15 18:36:30 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main( int argc, char **argv )
{
	(void)argv;

	if (argc != 2)
		return (err_msg(NULL, ERR_USAGE), FAILURE);
	return (SUCCESS);	
}
