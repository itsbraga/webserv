/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/30 14:39:27 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	---------------------- [ Object manipulation ] -----------------------
*/
Parser::Parser( std::string arg )
{
	try {
		_checkPath(arg);

		std::ifstream infile;
		infile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
		infile.open( arg.c_str() );
		if (infile.peek() == EOF)
			throw std::invalid_argument( "Configuration file is empty" );

		_fillBuffer( infile );
		infile.close();
		_initKeywordMap();
		_initStatusCodesVector();
	}
	catch (const std::ios_base::failure& e) {
    	throw std::runtime_error( "Cannot open config file, please check permissions" );
	}
	catch (const std::exception& e) {
		throw ;
	}
}

Parser::~Parser() {}

/*
	-------------------------- [ P: Utilities ] --------------------------
*/
std::string		Parser::_checkPath( std::string path )
{
	if (path.empty())
		throw std::invalid_argument( "Empty path" );
	
	struct stat buff;
	if (stat( path.c_str(), &buff ) == -1)
		throw std::invalid_argument( strerror( errno ) );
	if (S_ISDIR( buff.st_mode ))
		throw std::invalid_argument( "Is a directory" );
	if (path.substr( path.find_last_of( '.' ) ) != ".conf" )
		throw std::invalid_argument( "Invalid file extension" );
	return (path);
}

void	Parser::_fillBuffer( const std::ifstream& infile )
{
	std::ostringstream	buffer;
	buffer << infile.rdbuf();

	_buffer = buffer.str();
}

/*
	----------------------- [ P: Initializations ] -----------------------
*/
void	Parser::_initStatusCodesVector()
{
	_status_codes.push_back( 200 );
	_status_codes.push_back( 201 );
	_status_codes.push_back( 204 );
	_status_codes.push_back( 301 );
	_status_codes.push_back( 302 );
	_status_codes.push_back( 307 );
	_status_codes.push_back( 308 );
	_status_codes.push_back( 400 );
	_status_codes.push_back( 403 );
	_status_codes.push_back( 404 );
	_status_codes.push_back( 405 );
	_status_codes.push_back( 410 );
	_status_codes.push_back( 411 );
	_status_codes.push_back( 413 );
	_status_codes.push_back( 414 );
	_status_codes.push_back( 418 );
	_status_codes.push_back( 429 );
	_status_codes.push_back( 500 );
	_status_codes.push_back( 501 );
	_status_codes.push_back( 502 );
	_status_codes.push_back( 503 );
	_status_codes.push_back( 504 );
	_status_codes.push_back( 505 );
}

void	Parser::_initKeywordMap()
{
	_keywords["server"] = K_SERVER;
	_keywords["location"] = K_LOCATION;
	_keywords["listen"] = K_LISTEN;
	_keywords["server_name"] = K_SERVERNAME;
	_keywords["root"] = K_ROOT;
	_keywords["index"] = K_INDEX;
	_keywords["error_page"] = K_ERRORPAGE;
	_keywords["allowed_methods"] = K_ALLOWEDMETHODS;
	_keywords["client_max_body_size"] = K_CLIENTMAXSIZEBODY;
	_keywords["cgi"] = K_CGI;
	_keywords["autoindex"] = K_AUTOINDEX;
	_keywords["upload_allowed"] = K_UPLOADALLOWED;
	_keywords["return"] = K_RETURN;
	_keywords["on"] = K_ON;
}

/*
	----------------------- [ P: Token creation ] ------------------------
*/
Token	Parser::_createToken( std::string& value ) const
{
	TokenType type;
	lowerStr( value );

	if (isKeyword( value ))
		type = identifyKeyword( value );
	else if (isSymbol( value ))
		type = identifySymbol( value );
	else if (isValue( value ))
		type = identifyValue( value );
	else
		type = UNKNOWN;
	
	Token token( type, value );
	return (token);
}

void	Parser::_createTokenDelimiter( std::string::const_iterator it )
{
	std::string token_value( it, it + 1 );
	_tokens.push_back( _createToken( token_value ) );
}

/*
	-------------------------- [ Tokenization ] --------------------------
*/
void	Parser::bufferTokenize()
{
	std::string::const_iterator start_tok = _buffer.begin();
	std::string::const_iterator	end_tok;

	while (start_tok != _buffer.end())
	{
		end_tok = start_tok;
		while (end_tok != _buffer.end() && !isSymbol( *end_tok ) && !isWhiteSpace( *end_tok ))
			end_tok++;

		std::string	token_value( start_tok, end_tok );

		if (start_tok != end_tok)
		{
			Token token( _createToken( token_value ) );
			_tokens.push_back( token );
		}
		if (end_tok != _buffer.end())
		{
			if (isSymbol( *end_tok ))
				_createTokenDelimiter( end_tok );
		}
		start_tok = end_tok;
		if (start_tok != _buffer.end())
			++start_tok;
	}
}

/*
	--------------------- [ Main parsing function ] ----------------------
*/
void	Parser::parse()
{
	std::set<std::string>	locations_uri;
	std::set<std::string>	server_names;
	std::vector<Token>::const_iterator current = _tokens.begin();
	std::vector<Token>::const_iterator end = _tokens.end();

	enterContext( HTTP );
	
	while (current != end)
	{
		switch (current->getType())
		{
			case K_SERVER :
				if (getCurrentContext() != HTTP)
				 	throw SyntaxErrorException( "A SERVER_BLOCK is only expected in a HTTP context" );
				else if (peekType( current, 1 ) != S_LBRACE)
					throw SyntaxErrorException( "The keyword SERVER need to be followed by a LBRACE token" );
				enterContext( SERVER_BLOCK );
				current += 2;
				break;
			
			case K_LOCATION :
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "A LOCATION_BLOCK can't be outside a SERVER_BLOCK context" );
				else if (getCurrentContext() == LOCATION_BLOCK)
					throw SyntaxErrorException( "A LOCATION_BLOCK can't be inside another LOCATION_BLOCK (i wish that were the case)" );
				else if (peekType( current, 1 ) != V_PATH)
					throw SyntaxErrorException( "The keyword LOCATION need to be followed by a PATH token" );
				else if (!isValidPath((current + 1)->getValue()))
					throw ConfigurationErrorException( "The PATH after LOCATION keyword is not a valid path" );
				else if ((current + 1)->getValue()[0] != '/')
					throw ConfigurationErrorException( "The PATH after LOCATION must start with /" );
				else if (peekType( current, 2 ) != S_LBRACE)
					throw SyntaxErrorException( "A LOCATION_BLOCK can't be opened without a LBRACE token after PATH" );
				if (locations_uri.insert((current + 1)->getValue()).second == false)
					throw ConfigurationErrorException( "A LOCATION has been duplicated [" + (current + 1)->getValue() + "]");
				enterContext( LOCATION_BLOCK );
				current += 3;
				break;
			
			case K_LISTEN :
				if (getCurrentContext() != SERVER_BLOCK)
					throw SyntaxErrorException( "The keyword LISTEN is only expected in a SERVER_BLOCK context" );
				else if (peekType( current, 1 ) != V_NUMBER)
					throw SyntaxErrorException( "The keyword LOCATION need to be followed by an UNSIGNED NUMBER token" );
				else if (peekType( current, 2 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON token is missing after LISTEN keyword" );
				else if (!isValidPort( (current + 1)->getValue() ))
					throw ConfigurationErrorException( "The port given after LISTEN keyword is not a valid port" );
				current += 3;
				break;
			
			case K_ROOT :
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "The keyword ROOT is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_PATH)
					throw SyntaxErrorException( "The keyword ROOT need to be followed by a PATH token" );
				else if (!isValidPath((current + 1)->getValue()))
					throw ConfigurationErrorException( "The PATH after ROOT keyword is not a valid path" );
				else if (peekType( current, 2 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON token is missing after ROOT keyword" );
				current += 3;
				break;

			case K_INDEX :
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "The keyword ROOT is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_STR)
					throw SyntaxErrorException( "The keyword INDEX need to be followed by a STR token representing a file name" );
				else if (peekType( current, 2 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON token is missing after INDEX keyword" );
				current += 3;
				break;
			
			case K_SERVERNAME :
				if (getCurrentContext() != SERVER_BLOCK)
					throw SyntaxErrorException( "The keyword SERVER_NAME is only expected in a SERVER_BLOCK context" );
				else if (peekType( current, 1 ) != V_STR)
					throw SyntaxErrorException( "The keyword SERVER_NAME need to be followed by a STR token" );
				else if (peekType( current, 2 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON token is missing after SERVER_NAME keyword" );
				if (server_names.insert((current + 1)->getValue()).second == false)
					throw ConfigurationErrorException( "A SERVER_NAME has been set twice [" + ((current + 1)->getValue() + "]" ));
				current += 3;
				break;

			case K_ERRORPAGE : {
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "The keyword ERROR_PAGE is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_STATUSCODE)
						throw SyntaxErrorException( "The keyword ERROR_PAGE need to be followed by a STATUS_CODE token" );
				++current;
				int i = 0;
				while (current != end && current->getType() != V_PATH)
				{
					if (current->getType() != V_STATUSCODE)
						throw SyntaxErrorException( "Only STATUS_CODE are expected between ERROR_PAGE keyword and PATH" );
					if (!isErrorStatusCode( current->getValue() ))
						throw ConfigurationErrorException( "Only ERROR STATUS_CODE (400-599) are expected for ERROR_PAGE keyword" );
					else if (i >= 8)
						throw SyntaxErrorException( "No more than 8 STATUS_CODE can be assigned to only one ERROR_PAGE, you also need to check if PATH or SEMICOLON are missing" );
					++current, ++i;
				}
				if (current == end)
					throw SyntaxErrorException( "Unexpected end of file" );
				if (peekType( current, 1 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON token is missing after ERROR_PAGE keyword" );
				current += 2;
				break;
			}
				
			case K_UPLOADALLOWED :
				if (getCurrentContext() != LOCATION_BLOCK)
					throw SyntaxErrorException( "The keyword UPLOAD_ALLOWED is only expected in a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_PATH)
					throw SyntaxErrorException( "The keyword UPLOAD_ALLOWED need to be followed by a PATH token" );
				else if (!isValidPath( (current + 1)->getValue() ))
					throw ConfigurationErrorException( "The PATH after UPLOAD_ALLOWED keyword is not a valid path" );
				else if (peekType( current, 2 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON token is missing after UPLOAD_ALLOWED keyword" );
				current += 3;
				break;

			case K_CLIENTMAXSIZEBODY :
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "The keyword client_max_body_size is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_STR)
					throw SyntaxErrorException( "The keyword client_max_body_size need to be followed by a STR token" );
				else if (!isValidBodySize( (current + 1)->getValue() ))
					throw ConfigurationErrorException( "Value for client_max_body_size isn't valid" );
				else if (peekType( current, 2 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON token is missing after client_max_body_size keyword" );
				current += 3;
				break;
			
			case K_AUTOINDEX :
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "The keyword AUTO_INDEX is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != K_ON)
					throw SyntaxErrorException( "The keyword AUTO_INDEX need to be followed by an 'ON' keyword" );
				else if (peekType( current, 2 ) != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON is missing after AUTO_INDEX keyword" );
				current += 3;
				break;

			case K_ALLOWEDMETHODS : {
				if (getCurrentContext() != LOCATION_BLOCK)
					throw SyntaxErrorException( "The keyword ALLOWED_METHODS is only expected in a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_STR)
					throw SyntaxErrorException( "The keyword ALLOWED_METHODS need to be followed by a STR token" );
				++current;
				int i = 0;
				while (current != end && current->getType() != S_SEMICOLON)
				{
					if (current->getType() != V_STR)
						throw SyntaxErrorException( "Only STR are expected between ALLOWED_METHODS and SEMICOLON" );
					else if (!isValidMethod( current->getValue() ))
						throw ConfigurationErrorException( "There is a invalid METHOD name between ALLOWED_METHODS keyword and SEMICOLON" );
					else if (i >= 4)
						throw SyntaxErrorException( "No more than 4 METHODS can be allowed in a LOCATION_BLOCK context, you need also to check if SEMICOLON is missing" );
					++current, ++i;
				}
				if (current == end)
					throw SyntaxErrorException( "Unexpected end of file" );
				if (current->getType() != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON is missing after ALLOWED_METHODS keyword" );
				++current;
				break;
			}

			case K_CGI : {
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "The keyword CGI is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_EXTENSION)
					throw SyntaxErrorException( "The keyword CGI need to be followed by an EXTENSION keyword" );
				++current;
				int i = 0;
				while (current != end && current->getType() != S_SEMICOLON)
				{
					if (current->getType() != V_EXTENSION)
						throw SyntaxErrorException( "Only STR are expected between ALLOWED_METHODS and SEMICOLON" );
					else if (isValidExtension( current->getValue() ) == false)
						throw ConfigurationErrorException( "One of the extensions given after CGI keyword is incorrect" );
					else if (i >= 3)
						throw SyntaxErrorException( "No more than 3 different extensions can be allowed after a CGI keyword, you need also to check if SEMICOLON is missing" );
					++current, ++i;
				}
				if (current == end)
					throw SyntaxErrorException( "Unexpected end of file" );
				else if (current->getType() != S_SEMICOLON)
					throw SyntaxErrorException( "A SEMICOLON is missing after CGI keyword" );
				++current;
				break;
			}

			case K_RETURN : {
				if (!isInContext( SERVER_BLOCK ))
					throw SyntaxErrorException( "The keyword RETURN is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context" );
				else if (peekType( current, 1 ) != V_STATUSCODE)
					throw SyntaxErrorException( "The keyword RETURN need to be followed by a STATUS_CODE token" );
				else if (!isReturnStatusCode( (current + 1)->getValue() ))
					throw ConfigurationErrorException( "The STATUS_CODE given after RETURN token isn't accepted for a return directive" );
				std::string status_code_str = (current + 1)->getValue();
				if (status_code_str == "301" || status_code_str == "302" || status_code_str == "307" || status_code_str == "308")
				{
					if (peekType( current, 2 ) != V_PATH)
						throw ConfigurationErrorException( "A PATH is needed between 301 STATUS_CODE and SEMICOLON for a RETURN" );
					else if (!isValidPath((current + 2)->getValue()))
						throw ConfigurationErrorException( "A PATH after RETURN keyword is not a valid path" );
					if (peekType( current, 3 ) != S_SEMICOLON)
						throw SyntaxErrorException( "A SEMICOLON is missing after RETURN keyword" );
					current += 4;
				}
				else
				{
					if (peekType( current, 2 ) != S_SEMICOLON)
						throw SyntaxErrorException( "A SEMICOLON is missing after RETURN keyword" );
					current += 3;
				}
				break;
			}

			case S_RBRACE : 
				if (getCurrentContext() == HTTP)
					throw SyntaxErrorException( "Trying to close a BLOCK when no-one is open" );
				else
				{
					if (getCurrentContext() == SERVER_BLOCK)
						locations_uri.clear();
					exitContext();
					++current;
				}
				break;
			
			default :
				throw SyntaxErrorException( "An unknown syntax error has occured [" + current->getValue() + "]");	
					
		}
	}
	if (getCurrentContext() != HTTP)
		throw SyntaxErrorException( "A BLOCK has not been closed properly" );
}

/*
	-------------------- [ Server objects creation ] ---------------------
*/
void		Parser::createAllObjects( Webserv& webserv )
{
	ServerConfig	current_server;
	Location		current_location;

	std::vector<Token>::const_iterator current = _tokens.begin();
	std::vector<Token>::const_iterator end = _tokens.end();

	enterContext( HTTP );

	while (current != end)
	{
		switch (current->getType())
		{
			case K_SERVER :
				current_server = ServerConfig();

				enterContext( SERVER_BLOCK );
				current += 2;
				break;

			case K_LOCATION : {
				current_location = Location();
				std::string path = normalizePath( (current + 1)->getValue() );

				enterContext( LOCATION_BLOCK );
				current_location.setUri( path );
				current += 3;
				break;
			}
			
			case K_LISTEN : {
				unsigned short value;
				std::stringstream ss( (current + 1)->getValue() );
				ss >> value;

				current_server.setPort( value );
				current += 3;
				break;
			}

			case K_ROOT : {
				std::string path = normalizePath((current + 1)->getValue());

				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setRoot( path );
				else
					current_location.setRoot( path );
				current += 3;
				break;
			}
			
			case K_INDEX :
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setIndex( (current + 1)->getValue() );
				else
					current_location.setIndex( (current + 1)->getValue() );
				current += 3;
				break;
			
			case K_SERVERNAME :
				current_server.setServerName( (current + 1)->getValue() );
				current += 3;
				break;

			case K_ERRORPAGE : {
				++current;
				std::vector<int> status;
				std::string	file;
				int value;

				while (current->getType() != V_PATH)
				{
					std::stringstream ss(current->getValue());
					ss >> value;
					status.push_back(value);
					++current;
				}
				file = current->getValue();

				ErrorPage err_page(status, file);
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.getErrorPages().push_back( err_page );
				else
					current_location.getErrorPages().push_back( err_page );
				current += 2;
				break;
			}

			case K_UPLOADALLOWED : {
				std::string path = normalizePath((current + 1)->getValue());

				current_location.setUploadAllowed( true );
				current_location.setUploadPath( path ); 
				current += 3;
				break;
			}
			
			case K_CLIENTMAXSIZEBODY :
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setClientMaxSizeBody( (current + 1)->getValue() );
				else
					current_location.setClientMaxSizeBody( (current + 1)->getValue() );
				current += 3;
				break;

			case K_AUTOINDEX :
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setAutoIndex( true );
				else
					current_location.setAutoIndex( true );
				current += 3;
				break;
			
			case K_ALLOWEDMETHODS : {
				++current;
				std::vector<std::string> methods;

				while (current->getType() != S_SEMICOLON)
				{
					std::string method = current->getValue();
					methods.push_back( method );
					++current;
				}
				current_location.setAllowedMethods( methods );
				++current;
				break;
			}

			case K_CGI : {
				++current;
				
				std::vector<std::string>	cgi_extension;
				while(current->getType() != S_SEMICOLON)
				{
					cgi_extension.push_back( current->getValue() );
					++current;
				}
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setCgiExtension(cgi_extension);
				else
					current_location.setCgiExtension(cgi_extension);
				++current;
				break;
			}

			case K_RETURN : {
				unsigned int return_code;
				std::string path;
				std::stringstream ss( (current + 1)->getValue() );
				ss >> return_code;

				if (getCurrentContext() == SERVER_BLOCK)
				{
					current_server.setReturnCode( return_code );
					if (return_code == 301 || return_code == 302 || return_code == 307 || return_code == 308)
					{
						path = normalizePath((current + 2)->getValue());
						current_server.setReturnUri( path );
						current += 4;
					}
					else
						current += 3;
				}
				else
				{
					current_location.setReturnCode( return_code );
					if (return_code == 301 || return_code == 302 || return_code == 307 || return_code == 308)
					{
						path = normalizePath((current + 2)->getValue());
						current_location.setReturnUri( path );
						current += 4;
					}
					else
						current += 3;
				}
				break;
			}

			case S_RBRACE :
				if (getCurrentContext() == SERVER_BLOCK)
				{
					if (current_server.getPort() == 0 || current_server.getRoot().empty() == true)
						throw ConfigurationErrorException("A SERVER_BLOCK must have listen and root directives");
					webserv.addServerConfig( current_server );
					exitContext();
				}
				else
				{
					current_server.getLocations()[current_location.getUri()] = current_location;
					exitContext();
				}
				++current;
				break;
			
			default :
				throw ConfigurationErrorException( "An unknown and unexpected configuration error has occured" );
		}
	}
}

/*
	-------------------------- [ Identifiers ] ---------------------------
*/
TokenType	Parser::identifyKeyword( const std::string& to_identify ) const
{
	std::map<std::string, TokenType>::const_iterator it = _keywords.find( to_identify );

	if (it == _keywords.end())
		return (UNKNOWN);
	else
		return (it->second);
}

TokenType	Parser::identifySymbol( const std::string& to_identify ) const
{
	if (isLeftBrace( to_identify[0] ))
		return (S_LBRACE);
	else if (isRightBrace( to_identify[0] ))
		return (S_RBRACE);
	else
		return (S_SEMICOLON);
}

TokenType	Parser::identifyValue( const std::string& to_identify ) const
{
	if (isStatusCode( to_identify ))
		return (V_STATUSCODE);
	else if (isNumber( to_identify ))
		return (V_NUMBER);
	else if (isPath( to_identify ))
		return (V_PATH);
	else if (isExtension( to_identify ))
		return (V_EXTENSION);
	else
		return (V_STR);
}

/*
	-------------------------- [ Peek getters ] --------------------------
*/
TokenType	Parser::peekType( std::vector<Token>::const_iterator it, size_t offset ) const
{
	if (it + offset >= _tokens.end())
		throw SyntaxErrorException( "Unexpected end of file" );
	else
		return ((it + offset)->getType());
}

std::string		Parser::peekValue( std::vector<Token>::const_iterator it, size_t offset ) const
{
	if (it + offset >= _tokens.end())
		throw SyntaxErrorException( "Unexpected end of file" );
	else
		return ((it + offset)->getValue());
}

/*
	---------------------------- [ Context ] -----------------------------
*/
void	Parser::enterContext( Context context )
{
	_context_stack.push_back( context );
}

void	Parser::exitContext()
{
	if (_context_stack.empty())
		return ;

	_context_stack.pop_back();
}

bool	Parser::isInContext( const Context& ctx ) const
{
	std::vector<Context>::const_iterator it = _context_stack.begin();

	for (; it != _context_stack.end(); ++it)
	{
		if (*it == ctx)
			return (true);
	}
	return (false);
}

Context		Parser::getCurrentContext() const
 {
	if (_context_stack.empty())
		return (EMPTY);
	else
		return (_context_stack.back());
}

/*
	---------------------------- [ Operator ] ----------------------------
*/
std::ostream&	operator<<( std::ostream &os, const Parser& parser )
{
	std::vector<Token>::const_iterator it = parser.getTokens().begin();

	for (; it != parser.getTokens().end(); ++it)
		os << "[" RED << it->getTypeStr() << NC "[" YELLOW << it->getValue() << NC "]" "]" " - ";
	os << std::endl;
	return (os);
}