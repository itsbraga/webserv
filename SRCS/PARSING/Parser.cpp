/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/20 20:36:13 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	Parser::HandleFileConfig(char *arg, webserv_s *data)
{
	Parser* parser = NULL;
	try
	{
		parser = new Parser(arg);
		
	}
	catch (const std::exception &)
	{
		if (parser != NULL)
			delete parser;
		throw;
	}
}

Parser::Parser(char *arg)
{
	try
	{
		this->_conf_path = this->CheckPath(arg);
	}
	catch (std::exception &)
	{
		throw;
	}
	std::ifstream file(this->_conf_path);
	if (!file.is_open())
		throw std::runtime_error("Creation of an ifstream failed");
	std::ostringstream ss;
	ss << file.rdbuf();
	if (file.bad() == true)
		std::runtime_error("Rdbuf function failed");
	this->_buffer = ss.str();
	file.close();
}

std::string	Parser::CheckPath(char *arg)
{
	std::string path = arg;
	if (path.empty())
		throw std::invalid_argument("No file path");
	
	struct stat buff;
	if (stat(arg, &buff) == -1)
		throw std::invalid_argument(strerror(errno));
	else
	{
		if (S_ISDIR(buff.st_mode))
			throw std::invalid_argument("Is a directory");
	}
	return (path);
}

std::stringstream	Parser::CreateStringStream( void )
{
	std::stringstream ss(this->_buffer);
	return (ss);
}
