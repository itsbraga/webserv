/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/31 02:47:41 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

static bool __checkTimeOutDefine()
{
	if (CGI_INACTIVITY_TIMEOUT >= CLIENT_INACTIVITY_TIMEOUT)
		return (false);
	else if (CGI_SLOWLORIS_TIMEOUT >= CLIENT_SLOWLORIS_TIMEOUT)
		return (false);
	return (true);
}

static bool	__initUtilities( Webserv& webserv )
{
	if (!webserv.initListeners())
		return (false);

	init_method_map();
	Response::initBuilders();
	Response::initContentTypes();
	return (true);
}

static std::string	__loadConfigPath( int argc, char **argv )
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
	else if (__checkTimeOutDefine() == false)
		return (err_msg(NULL, "Bad timeout define"), FAILURE);
	
	Webserv		webserv;
	std::string config_path = __loadConfigPath( argc, argv );

	try {
		Parser parser( config_path );
		parser.bufferTokenize();
		// std::cout << parser << std::endl;
		parser.parse();
		std::cout << BOLD P_GREEN "PARSING OK\n" NC << std::endl;
		parser.createAllObjects( webserv );

		if (!__initUtilities( webserv ))
			return (FAILURE);
		if (!webserv.initEpoll())
			return (FAILURE);

		webserv.run();
	}
	catch (const ChildErrorException& e) {
		if (e.getErrorCode() == 502)
			return (CGI_ERROR);
		else
			return (INTERNAL_ERROR);
	}
	catch (const std::exception& e) {
		return (err_msg( "Parser", e.what() ), FAILURE);
	}
	return (SUCCESS);
}