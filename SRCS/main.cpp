/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/14 00:03:38 by panther          ###   ########.fr       */
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
	Webserv	webserv;

	if (webserv.addServer( "destination_finale", 8080 ) == false)
		return (FAILURE);
	if (webserv.init() == false)
		return (FAILURE);
	webserv.run();
	return (SUCCESS);
}