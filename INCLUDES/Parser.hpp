/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/19 03:51:36 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"

# include <cstdlib>
# include <iostream>
# include <fstream>
# include <sstream> 
# include <vector>
# include <map>
# include <set>
# include <string>
# include <cstring>
# include <cctype>
# include <cstdio>
# include <ctime>
# include <sys/stat.h>

enum Context
{
	HTTP,
	SERVER_BLOCK,
	LOCATION_BLOCK,

	EMPTY
};

enum TokenType
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
	K_CLIENTMAXSIZEBODY,
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
	V_EXTENSION,
	V_STATUSCODE,
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
		Token(const Token& to_copy) {this->_type = to_copy.getType(); this->_value = to_copy.getValue();}
		~Token() {}

		void			setType(TokenType type) {this->_type = type;}
		void			setValue(std::string value) {this->_value = value;}
		TokenType		getType( void ) const {return this->_type;}
		std::string		getTypeStr( void ) const;
		std::string		getValue( void ) const {return this->_value;}

};

class Parser
{
	private:
		std::string				_conf_path;
		std::string				_buffer;
		std::vector<Context>	_context_stack;
		std::vector<Token>		_tokens;
		std::map<std::string, TokenType>	_keywords;
		std::vector<unsigned int>	_status_codes;

	public:
		Parser(char* arg);
		~Parser(){}
		
		// static void 		handleFileConfig(char *arg, webserv_s *data);
		std::string 		checkPath(char *arg);
		void				initKeywordMap( void );
		void				initStatusCodesVector( void );
		void				bufferTokenize( void );
		void				parse( void );
		void				createAllObjects(webserv_t *webserv);
		void				fillBuffer(const std::ifstream &infile);
		Token				createToken(std::string value) const;
		void				createTokenDelimiter(std::string::const_iterator it);
		static	void		lowerStr(std::string& str);

		TokenType			identifyKeyword(const std::string& to_identify) const;
		TokenType			identifySymbol(const std::string& to_identify) const;
		TokenType			identifyValue(const std::string& to_identify) const;

		bool				isLeftBrace(char c) const ;
		bool				isRightBrace(char c) const ;
		bool				isSemiColon(char c) const ;
		bool				isServer(const std::string& to_compare) const;
		bool				isLocation(const std::string& to_compare) const;
		bool				isWhiteSpace(char c) const;
		bool				isKeyword(const std::string& to_compare) const;
		bool				isSymbol(const char c) const;
		bool				isSymbol(const std::string&	to_compare) const;
		bool				isValue(const std::string& to_compare) const;
		bool				isNumber(const std::string& to_compare) const;
		bool				isString(const std::string&	to_compare) const;
		bool				isPath(const std::string& to_compare) const;
		bool 				isExtension(const std::string& to_compare) const;
		bool 				isStatusCode(const std::string& to_compare) const;
		bool 				isErrorStatusCode(const std::string& to_compare) const;
		bool				isReturnStatusCode(const std::string& to_compare) const;
		bool				isValidPort(const std::string& to_check) const;
		bool				isValidMethod(const std::string& to_check) const;
		bool 				isValidBodySize(const std::string& value) const;

		TokenType			peekType(std::vector<Token>::const_iterator it, size_t offset) const;
		TokenType			peekValue(std::vector<Token>::const_iterator it, size_t offset) const;

		void				enterContext(Context ctx);
		void				exitContext( void );
		bool 				isInContext(Context ctx) const;
		Context				getCurrentContext( void ) const;

		std::string						getConfPath( void );
		std::string&					getBuffer( void );
		std::vector<Token>&				getTokens();
		const std::vector<Token>&		getTokens() const;
		std::vector<Context>			getContextStack( void );

		class SyntaxErrorException : public std::exception
		{
			private:
				std::string _detail;
			
			public:
				explicit SyntaxErrorException( const std::string& detail ) : _detail("Syntax Error: " + _detail) {}
				virtual ~SyntaxErrorException() throw() {}
				virtual const char *what( void ) const throw()
				{ return (_detail.c_str()); }	
		};

		class ConfigurationErrorException : public std::exception
		{
			private:
				std::string _detail;

			public:
				explicit ConfigurationErrorException( const std::string& detail) : _detail("Configuration Error: " + _detail) {}
				virtual ~ConfigurationErrorException() throw() {}
				virtual const char *what( void ) const throw()
				{ return (_detail.c_str()); }
		};
		
};

inline std::ostream&	operator<<(std::ostream &os, const Parser& to_insert)
{
	std::vector<Token>::const_iterator it = to_insert.getTokens().begin();
	for (; it != to_insert.getTokens().end(); ++it)
	{
		os << "[" << RED << it->getTypeStr() << RESET << "[" << YELLOW << it ->getValue() << RESET << "]" << "]" << " - ";
	}
	os << std::endl;
	return (os);
}

// - [server[KEYWORD]]