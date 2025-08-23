/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/23 20:03:31 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef enum Context
{
	HTTP,
	SERVER_BLOCK,
	LOCATION_BLOCK,
};

class Token
{
	private:
		unsigned int	_type;
		std::string		_value; //template ?

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
		std::string	_buffer;
		std::vector<Context>	_context_stack;
		std::vector<Token>		_tokens;
		std::set<std::string>	_keywords;

	public:
		Parser(char* arg);
		~Parser(){}
		
		static void 		handleFileConfig(char *arg, webserv_s *data);
		std::string 		checkPath(char *arg);
		void				initKeywordSet( void );
		void				bufferTokenize( void );
		Token				createToken(unsigned int type, std::string value);
		std::stringstream	createStringStream( void );

		bool				isLeftBrace(char c);
		bool				isRightBrace(char c);
		bool				isSemiColon(char c);
		bool				isKeyword(const std::string& to_compare);
		bool				isNumber(const std::string& to_compare);
		bool				isServer(const std::string& to_compare);
		bool				isLocation(const std::string& to_compare);

		void				enterContext(Context context);
		void				exitContext( void );
		Context				getCurrentContext( void );

		std::string				getConfPath( void );
		std::string				getBuffer( void );
		std::vector<Context>	getContextStack( void );

		class SyntaxError : public std::exception
		{
			const char *what( void ) const throw();	
		};
		
};