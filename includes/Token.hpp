/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:22:58 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/23 19:33:40 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Enum
\**************************/

enum TokenType
{
	// Keywords
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

	// Symbols
	S_LBRACE,
	S_RBRACE,
	S_SEMICOLON,

	// Values
	V_NUMBER,
	V_STR,
	V_PATH,
	V_EXTENSION,
	V_STATUSCODE,

	// Others
	UNKNOWN
};

/**************************\
 *	Class
\**************************/

class Token
{
	private:
		TokenType		_type;
		std::string		_value;

	public:			
		Token( TokenType type, std::string value ) : _type( type ), _value( value ) {}
		Token( const Token& toCopy ) : _type( toCopy._type ), _value( toCopy._value ) {}
		~Token() {}

		void			setType( TokenType type );
		void			setValue( std::string value );

		std::string		getTypeStr() const;
		TokenType		getType() const			{ return (_type); }
		std::string		getValue() const		{ return (_value); }

};