/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/19 06:11:46 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
std::

std::ifstream	check_path(char *arg)
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
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Ifstream failed");
	return file;
}

void	handle_file_config(char *arg, webserv_s *data)
{
	try
	{
		std::ifstream file = check_path(arg);
	}
	catch (const std::exception &e)
	{
		throw;
	}
}