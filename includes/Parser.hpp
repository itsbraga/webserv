/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/31 05:50:02 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used library
\**************************/

# include "Token.hpp"

/**************************\
 *	Enum
\**************************/

enum Context
{
	HTTP,
	SERVER_BLOCK,
	LOCATION_BLOCK,
	EMPTY
};

/**************************\
 *	Exceptions
\**************************/

class SyntaxErrorException : public std::exception
{
	private:
		std::string _detail;
	
	public:
		explicit SyntaxErrorException( const std::string& detail ) : _detail( "Syntax Error: " + detail ) {}
		virtual ~SyntaxErrorException() throw() {}

		virtual const char	*what() const throw()	{ return (_detail.c_str()); }	
};

class ConfigurationErrorException : public std::exception
{
	private:
		std::string _detail;

	public:
		explicit ConfigurationErrorException( const std::string& detail ) : _detail( "Configuration Error: " + detail ) {}
		virtual ~ConfigurationErrorException() throw() {}

		virtual const char	*what() const throw()	{ return (_detail.c_str()); }
};

/**************************\
 *	Class
\**************************/

class Webserv;

class Parser
{
	private:
		std::string							_conf_path;
		std::string							_buffer;
		std::vector<Context>				_context_stack;
		std::vector<Token>					_tokens;
		std::map<std::string, TokenType>	_keywords;
		std::vector<unsigned int>			_status_codes;

		std::string			_checkPath( std::string path );
		void				_fillBuffer( const std::ifstream& infile );
		void				_initKeywordMap();
		void				_initStatusCodesVector();
		Token				_createToken( std::string& value ) const;
		void				_createTokenDelimiter( std::string::const_iterator it );

	public:
		Parser( std::string arg );
		~Parser();

		std::string&					getConfPath()		{ return (_conf_path); }
		std::string&					getBuffer()			{ return (_buffer); }
		std::vector<Token>&				getTokens()			{ return (_tokens); }
		const std::vector<Token>&		getTokens() const	{ return (_tokens); }
		std::vector<Context>&			getContextStack()	{ return (_context_stack); }

		void				bufferTokenize();
		void				parse();
		void				createAllObjects( Webserv& webserv );

		std::string			normalizePath( const std::string& path ) const;

		TokenType			identifyKeyword( const std::string& to_identify ) const;
		TokenType			identifySymbol( const std::string& to_identify ) const;
		TokenType			identifyValue( const std::string& to_identify ) const;

		bool				isLeftBrace( const char c ) const;
		bool				isRightBrace( const char c ) const;
		bool				isSemiColon( const char c ) const;
		bool				isWhiteSpace( const char c ) const;
		bool				isSymbol( const char c ) const;
		bool				isServer( const std::string& to_compare ) const;
		bool				isLocation( const std::string& to_compare ) const;
		bool				isKeyword( const std::string& to_compare ) const;
		bool				isSymbol( const std::string& to_compare ) const;
		bool				isValue( const std::string& to_compare ) const;
		bool				isNumber( const std::string& to_compare ) const;
		bool				isString( const std::string& to_compare ) const;
		bool				isPath( const std::string& to_compare ) const;
		bool 				isExtension( const std::string& to_compare ) const;
		bool 				isStatusCode( const std::string& to_compare ) const;
		bool 				isErrorStatusCode( const std::string& to_compare ) const;
		bool				isReturnStatusCode( const std::string& to_compare ) const;
		bool				isValidPort( const std::string& to_check ) const;
		bool				isValidMethod( const std::string& to_check ) const;
		bool 				isValidBodySize( const std::string& value ) const;
		bool				isValidExtension( const std::string& to_compare ) const;
		bool				isValidPath( const std::string& to_check) const;

		TokenType			peekType( std::vector<Token>::const_iterator it, size_t offset ) const;
		std::string			peekValue( std::vector<Token>::const_iterator it, size_t offset ) const;

		void				enterContext( Context ctx );
		void				exitContext();
		bool 				isInContext( const Context& ctx ) const;
		Context				getCurrentContext() const;
};

/**************************\
 *	Operator
\**************************/

std::ostream&	operator<<( std::ostream &os, const Parser& parser );
