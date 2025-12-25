/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/25 18:57:18 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static void	__initUtilities()
{
	init_method_map();
	Response::initBuilders();
	Response::initContentTypes();
}

int	main( int argc, char **argv )
{
	// (void)argc;
	if (argc != 2)
		return (err_msg( NULL, "Configuration file missing" ), FAILURE);

	Webserv	webserv;
	
	try {
		Parser parser( argv[1] );
		parser.bufferTokenize();
		std::cout << parser << std::endl;
		parser.parse();
		std::cout << BOLD P_GREEN "PARSING OK\n" << std::endl;
		parser.createAllObjects( webserv );
	}
	catch (const std::exception& e) {
		err_msg( "Parser", e.what() );
		return (FAILURE);
	}

	if (!webserv.initEpoll())
		return (FAILURE);

	__initUtilities();
	webserv.run();
	
	return (SUCCESS);
}