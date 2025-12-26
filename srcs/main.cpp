/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/26 15:30:02 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static bool	__initUtilities( Webserv& webserv )
{
	if (!webserv.initListeners())
		return (false);

	init_method_map();
	Response::initBuilders();
	Response::initContentTypes();
	return (true);
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
		std::cout << BOLD P_GREEN "PARSING OK\n" NC << std::endl;
		parser.createAllObjects( webserv );
	}
	catch (const std::exception& e) {
		err_msg( "Parser", e.what() );
		return (FAILURE);
	}

	if (!__initUtilities( webserv ))
		return (FAILURE);

	if (!webserv.init())
		return (FAILURE);
	webserv.run();
	return (SUCCESS);
}