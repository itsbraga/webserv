/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/27 02:49:57 by pmateo           ###   ########.fr       */
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

static std::string	__loadConfigPath(int argc, char **argv)
{
	if (argc == 2)
		return (argv[1]);
	else
		return ("./file_conf/default.conf");
}

int	main( int argc, char **argv )
{
	if (argc > 2)
		return (err_msg( NULL, "Too many arguments" ), FAILURE);
	
	Webserv		webserv;
	std::string config_path = __loadConfigPath( argc, argv );
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