/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/05 01:51:05 by art3mis          ###   ########.fr       */
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

	if (webserv.addServer(8080, "destination_finale") == false)
		return (FAILURE);
	if (webserv.init() == false)
		return (FAILURE);
	webserv.run();
	return (SUCCESS);
}