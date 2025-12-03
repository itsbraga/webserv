/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/03 18:20:58 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

// int	main( int argc, char **argv )
// {
// 	webserv_t	conf;

// 	if (argc != 2)
// 		return (err_msg(NULL, ERR_USAGE), FAILURE);
// 	try {
// 		Parser::handleFileConfig(argv[1], &conf);
// 	}
// 	catch (const std::exception& e) {
// 		std::cerr << e.what() << '\n';
// 	}
// 	return (SUCCESS);
// }

int	main( void )
{
	Server server(8080, "destination_finale");
	if (server.init() == FAILURE)
		return (FAILURE);
	
}