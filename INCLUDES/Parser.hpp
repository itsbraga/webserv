/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/21 20:50:54 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Token
{
	private:
		unsigned int	_type;
		std::string		_value;

	public:			
		Token(unsigned int type, std::string value) : _type(type), _value(value) {}
		~Token(){}

		void			setType(unsigned int type);
		void			setValue(std::string value);
		unsigned int	getType( void );
		std::string		getValue( void );

};

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