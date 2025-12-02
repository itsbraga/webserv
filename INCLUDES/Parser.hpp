/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:56:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 19:41:02 by annabrag         ###   ########.fr       */
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
		Token( unsigned int type, std::string value ) : _type(type), _value(value) {}
		~Token(){}

		void			setType( unsigned int type );
		void			setValue( std::string value );
		unsigned int	getType();
		std::string		getValue();

};

class Parser
{
	private:
		std::string				_conf_path;
		std::string				_buffer;
		std::vector<Context>	_context_stack;
		std::vector<Token>		_tokens;
		std::set<std::string>	_keywords;

	public:
		Parser( char *arg );
		~Parser(){}
		
		static void 		handleFileConfig( char *arg, webserv_s* data );
		std::string 		checkPath( char *arg);
		void				initKeywordSet();
		void				bufferTokenize();
		Token				createToken( unsigned int type, std::string value );
		std::stringstream	createStringStream();

		bool		isLeftBrace( char c ) { return (c == '{'); }
		bool		isRightBrace( char c ) { return (c== '}'); }
		bool		isSemiColon( char c ) { return (c == ';'); }
		bool		isKeyword( const std::string& to_compare );
		bool		isNumber( const std::string& to_compare );
		bool		isServer( const std::string& to_compare );
		bool		isLocation( const std::string& to_compare );

		void		enterContext(Context context);
		void		exitContext();
		Context		getCurrentContext();

		std::string				getConfPath() { return (this->_conf_path); }
		std::string				getBuffer() { return (this->_buffer); }
		std::vector<Context>	getContextStack() { return (this->_context_stack); }

		class SyntaxErrorException : public std::exception
		{
			const char	*what() const throw();
		};
};