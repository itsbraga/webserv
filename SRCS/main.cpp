/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:53:10 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/19 22:55:42 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main( int argc, char **argv )
{
	webserv_t	webserv;
	
	webserv.servers_nb = 0;
	if (argc != 2)
		return (EXIT_FAILURE);
	Parser* parser = NULL;
	try
	{
		parser = new Parser(argv[1]);
		parser->bufferTokenize();
		std::cout << *parser << std::endl;
		parser->parse();
		std::cout << "parse ok\n";
		parser->createAllObjects(&webserv);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	for (unsigned int i = 0; i < webserv.servers_nb; ++i)
	{
		std::cout << webserv.servers[i] << std::endl;
	}
	return (0);
}
