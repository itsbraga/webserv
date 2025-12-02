/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/02 17:36:05 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main( int argc, char **argv )
{
	webserv_t	conf;

	if (argc != 2)
		return (err_msg(NULL, ERR_USAGE), FAILURE);
	try {
		Parser::handleFileConfig(argv[1], &conf);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		
	}
	// init_method_map(); etc...
	// while (true) // pour epoll qui manage les servers
	// {
		
	// }
	// return (SUCCESS);
}
