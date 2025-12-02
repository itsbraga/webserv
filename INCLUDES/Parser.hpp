/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 19:13:52 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"

typedef enum Context
{
	HTTP,
	SERVER_BLOCK,
	LOCATION_BLOCK,
};

typedef enum TokenType
{
	//Keywords
	K_SERVER,
	K_LOCATION,
	K_LISTEN,
	K_SERVERNAME,
	K_ROOT,
	K_INDEX,
	K_ERRORPAGE,
	K_ALLOWEDMETHODS,
	K_CGI,
	K_AUTOINDEX,
	K_UPLOADALLOWED,
	K_RETURN,
	K_ON,
	//Symbols
	S_LBRACE,
	S_RBRACE,
	S_SEMICOLON,
	//Values
	V_NUMBER,
	V_STR,
	V_PATH,
	//Others
	UNKNOW
};

class Token
{
	private:
		TokenType		_type;
		std::string		_value;

	public:			
		Token(TokenType type, std::string value) : _type(type), _value(value) {}
		~Token(){}

		void			setType(TokenType type);
		void			setValue(std::string value);
		TokenType		getType( void );
		std::string		getValue( void );

};

class Parser
{
	private:
		std::string				_conf_path;
		std::string				_buffer;
		std::vector<Context>	_context_stack;
		std::vector<Token>		_tokens;
		std::map<std::string, TokenType>	_keywords;

	public:
		Parser(char* arg);
		~Parser(){}
		
		static void 		handleFileConfig(char *arg, webserv_s *data);
		std::string 		checkPath(char *arg);
		void				initKeywordMap( void );
		void				bufferTokenize( void );
		void				fillBuffer(const std::ifstream &infile);
		Token				createToken(std::string value) const;
		void				createTokenDelimiter(std::string::const_iterator it);
		std::stringstream	createStringStream( void );

		TokenType			identifyKeyword(const std::string& to_identify) const;
		TokenType			identifySymbol(const std::string& to_identify) const;
		TokenType			identifyValue(const std::string& to_identify) const;

		bool				isLeftBrace(char c) const ;
		bool				isRightBrace(char c) const ;
		bool				isSemiColon(char c) const ;
		bool				isWhiteSpace(char c) const;
		bool				isKeyword(const std::string& to_compare) const;
		bool				isSymbol(const std::string&	to_compare) const;
		bool				isValue(const std::string& to_compare) const;
		bool				isNumber(const std::string& to_compare) const;
		bool				isString(const std::string&	to_compare) const;
		bool				isPath(const std::string& to_compare) const;
		bool				isServer(const std::string& to_compare) const;
		bool				isLocation(const std::string& to_compare) const;

		void				enterContext(Context context);
		void				exitContext( void );
		Context				getCurrentContext( void );

		std::string						getConfPath( void );
		std::string&					getBuffer( void );
		std::vector<Token>&				getTokens();
		const std::vector<Token>&		getTokens() const;
		std::vector<Context>			getContextStack( void );

		class SyntaxError : public std::exception
		{
			const char *what( void ) const throw();	
		};
		
};

inline std::ostream&	operator<<(std::ostream &os, const Parser& to_insert)
{
	std::vector<std::string>::const_iterator it = to_insert.getTokens().begin();
	for (; it != to_insert.getTokens().end(); ++it)
	{
		os << RED << "[" << RESET << *it << RED << "]" << RESET << " - ";
	}
	os << std::endl;
	return (os);
}