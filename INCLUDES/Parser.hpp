/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/20 20:42:47 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Parser
{
	private:
		std::string	_conf_path;
		std::string _buffer;

	public:
		Parser(char*arg);
		~Parser(){}
		
		static void 		HandleFileConfig(char *arg, webserv_s *data);
		std::string 		CheckPath(char *arg);
		std::stringstream	CreateStringStream( void );

		class SyntaxError : public std::exception
		{
			const char *what( void ) const throw();	
		};

		
};