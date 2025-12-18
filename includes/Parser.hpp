/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/16 02:46:27 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

# include <string>
# include <vector>
# include <set>
# include <fstream>
# include <sstream>
# include <cctype>
# include <errno.h>

// class Token
// {
// 	private:
// 		unsigned int	_type;
// 		std::string		_value; //template ?

// 	public:			
// 		Token( unsigned int type, std::string value ) : _type(type), _value(value) {}
// 		~Token(){}

// 		void			setType( unsigned int type );
// 		void			setValue( std::string value );
// 		unsigned int	getType();
// 		std::string		getValue();
// };

enum Context
{
	HTTP,
	SERVER_BLOCK,
	LOCATION_BLOCK
};

/**************************\
 *	Class
\**************************/

class Parser
{
	private:
		std::string				_conf_path;
		std::string				_buffer;
		std::vector<Context>	_context_stack;
		// std::vector<Token>		_tokens;
		std::set<std::string>	_keywords;

		std::string 			_checkPath( char *arg );

	public:
		Parser( char *arg );
		~Parser() {}

		std::string				getConfPath()		{ return (_conf_path); }
		std::string				getBuffer()			{ return (_buffer); }
		std::vector<Context>	getContextStack()	{ return (_context_stack); }

		// static void 		handleFileConfig( char *arg, webserv_s* data );
		void				initKeywordSet();
		void				bufferTokenize();
		// Token			createToken( unsigned int type, std::string value );
		std::stringstream	createStringStream();

		bool				isLeftBrace( char c )	{ return (c == '{'); }
		bool				isRightBrace( char c )	{ return (c == '}'); }
		bool				isSemiColon( char c )	{ return (c == ';'); }

		bool				isKeyword( const std::string& to_compare );
		bool				isNumber( const std::string& to_compare );
		bool				isServer( const std::string& to_compare );
		bool				isLocation( const std::string& to_compare );

		void				enterContext(Context context);
		void				exitContext();
		Context				getCurrentContext();
};