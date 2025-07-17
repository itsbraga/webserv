/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/07/17 21:18:01 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int	main( int argc, char **argv )
{
	(void)argv;

	if (argc != 2)
		return (err_msg(NULL, ERR_USAGE), FAILURE);

	// initMethodMap(); etc...
	while (true) // pour epoll qui manage les servers
	{
		
	}
	return (SUCCESS);
}
