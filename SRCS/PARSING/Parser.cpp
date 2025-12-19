/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/19 23:33:12 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// void	Parser::handleFileConfig(char *arg, webserv_s *data)
// {
// 	Parser* parser = NULL;
// 	try
// 	{
// 		parser = new Parser(arg);
// 		parser->bufferTokenize();
// 	}
// 	catch (const std::exception &)
// 	{
// 		if (parser != NULL)
// 			delete parser;
// 		throw;
// 	}
// }

Parser::Parser(char *arg)
{
	try
	{
		this->checkPath(arg);
		std::ifstream infile;
		infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		infile.open(arg);
		if (infile.peek() == EOF)
			throw std::invalid_argument("Configuration file is empty !");
		this->fillBuffer(infile);
		infile.close();
		this->initKeywordMap();
	}
	catch(const std::exception& e)
	{
		throw;
	}
}

std::string	Parser::checkPath(char *arg)
{
	std::string path = arg;
	if (path.empty())
		throw std::invalid_argument("No file path");
	
	struct stat buff;
	if (stat(arg, &buff) == -1)
		throw std::invalid_argument(strerror(errno));
	else
	{
		if (S_ISDIR(buff.st_mode))
			throw std::invalid_argument("Is a directory");
	}
	if (path.substr(path.find_last_of('.')) != ".conf")
		throw std::invalid_argument("Invalid file extension");
	return (path);
}

void	Parser::bufferTokenize( void )
{
	std::string::const_iterator start_tok = this->_buffer.begin();
	std::string::const_iterator	end_tok;
	while (start_tok != this->_buffer.end())
	{
		end_tok = start_tok;
		while (end_tok != this->_buffer.end() 
			&& !isSymbol(*end_tok)
			&& !isWhiteSpace(*end_tok))
			end_tok++;
		std::string	token_value(start_tok, end_tok);
		if (start_tok != end_tok)
		{
			Token token(createToken(token_value));
			this->_tokens.push_back(token);
		}
		if (end_tok != this->_buffer.end())
		{
			if (isSymbol(*end_tok) == true)
				createTokenDelimiter(end_tok);
		}
		start_tok = end_tok;
		if (start_tok != this->_buffer.end())
			++start_tok;
	}
}

void	Parser::parse( void )
{
	std::vector<Token>::const_iterator current = _tokens.begin();
	std::vector<Token>::const_iterator end = _tokens.end();

	this->enterContext(HTTP);
	
	while (current != end)
	{
		switch (current->getType())
		{
			case K_SERVER :
				if (getCurrentContext() != HTTP)
				 	throw SyntaxErrorException("A SERVER_BLOCK is only expected in a HTTP context");
				else if (peekType(current, 1) != S_LBRACE)
					throw SyntaxErrorException("The keyword SERVER need to be followed by a LBRACE token");
				enterContext(SERVER_BLOCK);
				current += 2;
				break;
			
			case K_LOCATION :
				if (isInContext(SERVER_BLOCK) == false)
					throw SyntaxErrorException("A LOCATION_BLOCK can't be outside a SERVER_BLOCK context");
				else if (getCurrentContext() == LOCATION_BLOCK)
					throw SyntaxErrorException("A LOCATION_BLOCK can't be inside another LOCATION_BLOCK (i wish that were the case)");
				else if (peekType(current, 1) != V_PATH)
					throw SyntaxErrorException("The keyword LOCATION need to be followed by a PATH token");
				else if (peekType(current, 2) != S_LBRACE)
					throw SyntaxErrorException("A LOCATION_BLOCK can't be opened without a LBRACE token after PATH");
				enterContext(LOCATION_BLOCK);
				current += 3;
				break;
			
			case K_LISTEN :
				if (getCurrentContext() != SERVER_BLOCK)
					throw SyntaxErrorException("The keyword LISTEN is only expected in a SERVER_BLOCK context");
				else if (peekType(current, 1) != V_NUMBER)
					throw SyntaxErrorException("The keyword LOCATION need to be followed by an UNSIGNED NUMBER token");
				else if (peekType(current, 2) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON token is missing after LISTEN keyword");
				else if (isValidPort((current + 1)->getValue()) == false)
					throw ConfigurationErrorException("The port given after LISTEN keyword is not a valid port");
				current += 3;
				break;
			
			case K_ROOT :
				if (isInContext(SERVER_BLOCK) == false)
					throw SyntaxErrorException("The keyword ROOT is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_PATH)
					throw SyntaxErrorException("The keyword ROOT need to be followed by a PATH token");
				else if (peekType(current, 2) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON token is missing after ROOT keyword");
				current += 3;
				break;

			case K_INDEX :
				if (isInContext(SERVER_BLOCK) == false)
					throw SyntaxErrorException("The keyword ROOT is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_STR)
					throw SyntaxErrorException("The keyword INDEX need to be followed by a STR token representing a file name");
				else if (peekType(current, 2) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON token is missing after INDEX keyword");
				current += 3;
				break;
			
			case K_SERVERNAME :
				if (getCurrentContext() != SERVER_BLOCK)
					throw SyntaxErrorException("The keyword SERVER_NAME is only expected in a SERVER_BLOCK context");
				else if (peekType(current, 1) != V_STR)
					throw SyntaxErrorException("The keyword SERVER_NAME need to be followed by a STR token");
				else if (peekType(current, 2) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON token is missing after SERVER_NAME keyword");
				current += 3;
				break;

			case K_ERRORPAGE : {
				if (isInContext(SERVER_BLOCK) == false)
					throw SyntaxErrorException("The keyword ERROR_PAGE is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_STATUSCODE)
						throw SyntaxErrorException("The keyword ERROR_PAGE need to be followed by a STATUS_CODE token");
				++current;
				int i = 0;
				while (current != end && current->getType() != V_PATH)
				{
					if (current->getType() != V_STATUSCODE)
						throw SyntaxErrorException("Only STATUS_CODE are expected between ERROR_PAGE keyword and PATH");
					if (isErrorStatusCode(current->getValue()) == false)
						throw ConfigurationErrorException("Only ERROR STATUS_CODE (400-599) are expected for ERROR_PAGE keyword");
					else if (i >= 8)
						throw SyntaxErrorException("No more than 8 STATUS_CODE can be assigned to only one ERROR_PAGE, you also need to check if PATH or SEMICOLON are missing");
					++current, ++i;
				}
				if (current == end)
					throw SyntaxErrorException("Unexpected end of file");
				if (peekType(current, 1) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON token is missing after ERROR_PAGE keyword");
				current += 2;
				break;
			}
				
			case K_UPLOADALLOWED :
				if (getCurrentContext() != LOCATION_BLOCK)
					throw SyntaxErrorException("The keyword UPLOAD_ALLOWED is only expected in a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_PATH)
					throw SyntaxErrorException("The keyword UPLOAD_ALLOWED need to be followed by a PATH token");
				else if (peekType(current, 2) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON token is missing after UPLOAD_ALLOWED keyword");
				current += 3;
				break;

			case K_CLIENTMAXSIZEBODY :
				if (isInContext(SERVER_BLOCK) == false)
					throw SyntaxErrorException("The keyword CLIENT_MAX_SIZE_BODY is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_STR)
					throw SyntaxErrorException("The keyword CLIENT_MAX_SIZE_BODY need to be followed by a STR token");
				else if (isValidBodySize((current + 1)->getValue()) == false)
					throw ConfigurationErrorException("Value for CLIENT_MAX_SIZE_BODY isn't valid");
				else if (peekType(current, 2) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON token is missing after CLIENT_MAX_SIZE_BODY keyword");
				current += 3;
				break;
			
			case K_AUTOINDEX :
				if (isInContext(SERVER_BLOCK) == false)
					throw SyntaxErrorException("The keyword AUTO_INDEX is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context");
				else if (peekType(current, 1) != K_ON)
					throw SyntaxErrorException("The keyword AUTO_INDEX need to be followed by an 'ON' keyword");
				else if (peekType(current, 2) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON is missing after AUTO_INDEX keyword");
				current += 3;
				break;

			case K_ALLOWEDMETHODS : {
				if (getCurrentContext() != LOCATION_BLOCK)
					throw SyntaxErrorException("The keyword ALLOWED_METHODS is only expected in a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_STR)
					throw SyntaxErrorException("The keyword ALLOWED_METHODS need to be followed by a STR token");
				++current;
				int i = 0;
				while (current != end && current->getType() != S_SEMICOLON)
				{
					if (current->getType() != V_STR)
						throw SyntaxErrorException("Only STR are expected between ALLOWED_METHODS and SEMICOLON");
					else if (isValidMethod(current->getValue()) == false)
						throw ConfigurationErrorException("There is a invalid METHOD name between ALLOWED_METHODS keyword and SEMICOLON");
					else if (i >= 4)
						throw SyntaxErrorException("No more than 4 METHODS can be allowed in a LOCATION_BLOCK context, you need also to check if SEMICOLON is missing");
					++current, ++i;
				}
				if (current == end)
					throw SyntaxErrorException("Unexpected end of file");
				if (current->getType() != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON is missing after ALLOWED_METHODS keyword");
				++current;
				break;
			}

			case K_CGI :
				if (getCurrentContext() != LOCATION_BLOCK)
					throw SyntaxErrorException("The keyword CGI is only expected in a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_EXTENSION)
					throw SyntaxErrorException("The keyword CGI need to be followed by an EXTENSION keyword");
				else if (peekType(current, 2) != V_PATH)
					throw SyntaxErrorException("Only a PATH token is expected between EXTENSION and SEMICOLON for CGI");
				else if (peekType(current, 3) != S_SEMICOLON)
					throw SyntaxErrorException("A SEMICOLON is missing after CGI keyword");
				current += 4;
				break;

			case K_RETURN : 
				if (isInContext(SERVER_BLOCK) == false)
					throw SyntaxErrorException("The keyword RETURN is only expected in a SERVER_BLOCK or a LOCATION_BLOCK context");
				else if (peekType(current, 1) != V_STATUSCODE)
					throw SyntaxErrorException("The keyword RETURN need to be followed by a STATUS_CODE token");
				else if (isReturnStatusCode((current + 1)->getValue()) == false)
					throw ConfigurationErrorException("The STATUS_CODE given after RETURN token isn't accepted for a return directive");
				if ((current + 1)->getValue() == "301")
				{
					if (peekType(current, 2) != V_PATH)
						throw ConfigurationErrorException("A PATH is needed between 301 STATUS_CODE and SEMICOLON for a RETURN");
					if (peekType(current, 3) != S_SEMICOLON)
						throw SyntaxErrorException("A SEMICOLON is missing after RETURN keyword");
					current += 4;
				}
				else
				{
					if (peekType(current, 2) != S_SEMICOLON)
						throw SyntaxErrorException("A SEMICOLON is missing after RETURN keyword");
					current += 3;
				}
				break;

			case S_RBRACE : 
				if(getCurrentContext() == HTTP)
					throw SyntaxErrorException("Trying to close a BLOCK when no-one is open");
				else
				{
					exitContext();
					++current;
				}
				break;
			
			default :
				std::cout << "fautif = " << current->getValue() << std::endl;
				throw SyntaxErrorException("An unknow syntax error has occured");	
					
		}
	}
	if (getCurrentContext() != HTTP)
		throw SyntaxErrorException("A BLOCK has not been closed properly");
}

void		Parser::createAllObjects(webserv_t *webserv)
{
	Server 		current_server;
	Location	current_location;
	std::vector<Token>::const_iterator current = _tokens.begin();
	std::vector<Token>::const_iterator end = _tokens.end();

	enterContext(HTTP);

	while (current != end)
	{
		switch (current->getType())
		{
			case K_SERVER :
				current_server = Server();
				enterContext(SERVER_BLOCK);
				current += 2;
				break;

			case K_LOCATION :
				current_location = Location();
				enterContext(LOCATION_BLOCK);
				current_location.setUri((current + 1)->getValue());
				current += 3;
				break;
			
			case K_LISTEN : {
				unsigned short int value;
				std::stringstream ss((current + 1)->getValue());
				ss >> value;
				current_server.setPort(value);
				current += 3;
				break;
			}

			case K_ROOT :
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setRoot((current + 1)->getValue());
				else
					current_location.setRoot((current + 1)->getValue());
				current += 3;
				break;
			
			case K_INDEX :
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setIndex((current + 1)->getValue());
				else
					current_location.setIndex((current + 1)->getValue());
				current += 3;
				break;
			
			case K_SERVERNAME :
				current_server.setServerName((current + 1)->getValue());
				current += 3;
				break;

			case K_ERRORPAGE : {
				++current;
				std::vector<int> status;
				std::string	file;
				int value = 0;
				std::stringstream ss;
				while (current->getType() != V_PATH)
				{
					ss.str(current->getValue());
					ss >> value;
					status.push_back(value);
					++current;
				}
				file = current->getValue();
				ErrorPage epage(status, file);
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.getErrorPage().push_back(epage);
				else
					current_location.getErrorPage().push_back(epage);
				current += 2;
				break;
				
			}

			case K_UPLOADALLOWED :
				current_location.setUploadAllowed(true);
				current_location.setUploadPath((current + 1)->getValue());
				current += 3;
				break;
			
			case K_CLIENTMAXSIZEBODY :
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setClientMaxSizeBody((current + 1)->getValue());
				else
					current_location.setClientMaxSizeBody((current + 1)->getValue());
				current += 3;
				break;

			case K_AUTOINDEX :
				if (getCurrentContext() == SERVER_BLOCK)
					current_server.setAutoIndex(true);
				else
					current_location.setAutoIndex(true);
				current += 3;
				break;
			
			case K_ALLOWEDMETHODS : {
				++current;
				std::vector<std::string> methods;
				while (current->getType() != S_SEMICOLON)
				{
					methods.push_back(current->getValue());
					++current;
				}
				current_location.setAllowedMethods(methods);
				++current;
				break;
			}

			case K_CGI : {
				std::pair<std::string, std::string>	cgi_bin;
				cgi_bin = std::make_pair((current + 1)->getValue(), (current + 2)->getValue());
				current_location.getCgiBin().push_back(cgi_bin);
				current += 4;
				break;
			}

			case K_RETURN : {
				unsigned int return_code;
				std::stringstream ss((current + 1)->getValue());
				ss >> return_code;	
				if (getCurrentContext() == SERVER_BLOCK)
				{
					current_server.setReturnCode(return_code);
					if ((current + 1)->getValue() == "301")
					{
						current_server.setReturnUri((current + 2)->getValue());
						current += 4;
					}
					else
						current += 3;
				}
				else
				{
					current_location.setReturnCode(return_code);
					if ((current + 1)->getValue() == "301")
					{
						current_location.setReturnUri((current + 2)->getValue());
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
					webserv->servers.push_back(current_server);
					webserv->servers_nb += 1;
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
				throw ConfigurationErrorException("An unknow and unexpected configuration error has occured");
		}
	}
}

Token		Parser::createToken(std::string value) const
{
	TokenType	type;
	
	Parser::lowerStr(value);
	if (isKeyword(value) == true)
		type = identifyKeyword(value);
	else if (isSymbol(value) == true)
		type = identifySymbol(value);
	else if (isValue(value) == true)
		type = identifyValue(value);
	else
		type = UNKNOW;
	
	Token token(type, value);
	return (token);
}

TokenType	Parser::identifyKeyword(const std::string& to_identify) const
{
	std::map<std::string, TokenType>::const_iterator it = this->_keywords.find(to_identify);
	if (it == this->_keywords.end())
		return (UNKNOW);
	else
		return (it->second);
}

TokenType	Parser::identifySymbol(const std::string& to_identify) const
{
	if (isLeftBrace(to_identify[0]) == true)
		return (S_LBRACE);
	else if (isRightBrace(to_identify[0]) == true)
		return (S_RBRACE);
	else
		return (S_SEMICOLON);
}

TokenType	Parser::identifyValue(const std::string& to_identify) const
{
	if (isStatusCode(to_identify) == true)
		return (V_STATUSCODE);
	else if (isNumber(to_identify) == true)
		return (V_NUMBER);
	else if (isString(to_identify) == true)
		return (V_STR);
	else if (isPath(to_identify) == true)
		return (V_PATH);
	else
		return (V_EXTENSION);
}

void		Parser::createTokenDelimiter(std::string::const_iterator it)
{
	std::string token_value(it, it + 1);
	this->_tokens.push_back(createToken(token_value));
}

void		Parser::fillBuffer(const std::ifstream& infile)
{
	std::ostringstream	buffer;
	buffer << infile.rdbuf();
	this->_buffer = buffer.str();
}

// std::stringstream	Parser::createStringStream( void )
// {
// 	std::stringstream ss(this->_buffer);
// 	return (ss);
// }

void	Parser::initStatusCodesVector( void )
{
	this->_status_codes.push_back(200);
	this->_status_codes.push_back(201);
	this->_status_codes.push_back(202);
	this->_status_codes.push_back(301);
	this->_status_codes.push_back(400);
	this->_status_codes.push_back(403);
	this->_status_codes.push_back(404);
	this->_status_codes.push_back(405);
	this->_status_codes.push_back(411);
	this->_status_codes.push_back(414);
	this->_status_codes.push_back(418);
	this->_status_codes.push_back(429);
	this->_status_codes.push_back(500);
	this->_status_codes.push_back(501);
	this->_status_codes.push_back(502);
	this->_status_codes.push_back(503);
	this->_status_codes.push_back(504);
	this->_status_codes.push_back(505);
}

void	Parser::initKeywordMap( void )
{
	this->_keywords["server"] = K_SERVER;
	this->_keywords["location"] = K_LOCATION;
	this->_keywords["listen"] = K_LISTEN;
	this->_keywords["server_name"] = K_SERVERNAME;
	this->_keywords["root"] = K_ROOT;
	this->_keywords["index"] = K_INDEX;
	this->_keywords["error_page"] = K_ERRORPAGE;
	this->_keywords["allowed_methods"] = K_ALLOWEDMETHODS;
	this->_keywords["client_max_size_body"] = K_CLIENTMAXSIZEBODY;
	this->_keywords["cgi"] = K_CGI;
	this->_keywords["autoindex"] = K_AUTOINDEX;
	this->_keywords["upload_allowed"] = K_UPLOADALLOWED;
	this->_keywords["return"] = K_RETURN;
	this->_keywords["on"] = K_ON;
}

bool	Parser::isLeftBrace(char c) const
{
	return (c == '{');
}

bool	Parser::isRightBrace(char c) const
{
	return (c == '}');
}

bool	Parser::isSemiColon(char c) const
{
	return (c == ';');
}

bool	Parser::isWhiteSpace(char c) const
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		return (true);
	else
		return (false);
}

bool	Parser::isKeyword(const std::string& to_compare) const
{
	return (this->_keywords.find(to_compare) != this->_keywords.end());
}

bool	Parser::isSymbol(const char c) const
{
	return (c == '{' || c == '}' || c == ';');
}

bool	Parser::isSymbol(const std::string& to_compare) const
{
	return (to_compare == "{" || to_compare == "}" || to_compare == ";");
}

bool	Parser::isValue(const std::string& to_compare) const
{
	return (isNumber(to_compare) || isString(to_compare) \
			|| isPath(to_compare) || isExtension(to_compare) \
			|| isStatusCode(to_compare));
}

bool	Parser::isNumber(const std::string& to_compare) const
{
	std::string::const_iterator it;
	
	for (it = to_compare.begin(); it != to_compare.end(); it++)
	{
		if (isdigit(static_cast<int>(*it)) == 0)
			return(false);
	}
	return (true);
}

bool	Parser::isString(const std::string& to_compare) const
{
	std::string::const_iterator it;

	if (to_compare[0] == '.')
		return (false);
	for (it = to_compare.begin(); it != to_compare.end(); it++)
	{
		if (isalpha(static_cast<int>(*it)) == 0 && *it != '.')
			return (false);
	}
	return (true);
}

bool Parser::isPath(const std::string& to_compare) const
{
	if (to_compare.find('/') == std::string::npos)
		return (false);
	else
		return (true);
}

bool Parser::isExtension(const std::string& to_compare) const
{
	if (to_compare.find_last_of('.') == std::string::npos)
		return (false);
	else
		return (true);
}

bool Parser::isStatusCode(const std::string& to_compare) const
{
	if (isNumber(to_compare) == false && to_compare.length() != 3)
		return (false);
	std::stringstream ss(to_compare);
	unsigned int to_find;
	ss >> to_find;
	std::vector<unsigned int>::const_iterator it = _status_codes.begin();
	for (; it != _status_codes.end(); ++it)
	{
		if (*it == to_find)
			return (true);
	}
	return (false);
}

bool 	Parser::isErrorStatusCode(const std::string& to_compare) const
{
	unsigned int code;
	std::stringstream ss(to_compare);
	ss >> code;
	if (code >= 400 && code <= 599)
		return (true);
	else
		return (false);
}


bool	Parser::isReturnStatusCode(const std::string& to_compare) const
{
	if (to_compare == "301" || to_compare == "403" || to_compare == "404" \
		|| to_compare == "418" || to_compare == "503")
		return (true);
	else
		return (false);
}

bool	Parser::isValidPort(const std::string& to_check) const
{
	unsigned int port;
	std::stringstream ss(to_check);
	ss >> port;
	if (port == 0 || port > 65536)
		return false;
	else
		return true;
}

bool	Parser::isValidMethod(const std::string& to_check) const
{
	if (to_check == "get" || to_check == "head" || to_check == "post" || to_check == "delete")
		return (true);
	else
		return (false);
}

bool Parser::isValidBodySize(const std::string& value) const
{
    if (value[0] == '-')
        return false;
    
    size_t i = 0;
    
    while (i < value.length() && isdigit(value[i]))
        i++;
    
    if (i == 0)
        return false;
    else if (i == value.length())
        return true;
    else if (i != value.length() - 1)
        return false;

    char unit = value[i];
    return (unit == 'k' || unit == 'K' || 
            unit == 'm' || unit == 'M' || 
            unit == 'g' || unit == 'G');
}

bool	Parser::isServer(const std::string& to_compare) const
{
	return (to_compare == "server");
}

bool	Parser::isLocation(const std::string& to_compare) const
{
	return (to_compare == "location");
}

TokenType	Parser::peekType(std::vector<Token>::const_iterator it, size_t offset) const
{
	if (it + offset >= _tokens.end())
        throw SyntaxErrorException("Unexpected end of file");
    return (it + offset)->getType();
}

std::string		Parser::peekValue(std::vector<Token>::const_iterator it, size_t offset) const
{
	if (it + offset >= _tokens.end())
        throw SyntaxErrorException("Unexpected end of file");
    return (it + offset)->getValue();
}

void	Parser::enterContext(Context context)
{
	this->_context_stack.push_back(context);
}

void	Parser::exitContext( void )
{
	if (this->_context_stack.empty() == false)
		this->_context_stack.pop_back();
}

bool	Parser::isInContext(Context ctx) const
{
	std::vector<Context>::const_iterator it = _context_stack.begin();
	for (; it != _context_stack.end(); ++it)
	{
		if (*it == ctx)
			return (true);
	}
	return (false);
}

Context	Parser::getCurrentContext( void ) const
 {
	if (this->_context_stack.empty() == false)
		return (this->_context_stack.back());
	else
		return (EMPTY);
}

std::string	Parser::getConfPath( void )
{
	return (this->_conf_path);
}

std::string&	Parser::getBuffer()
{
	return (this->_buffer);
}

std::vector<Token>&	Parser::getTokens()
{
	return (this->_tokens);
}

const std::vector<Token>&	Parser::getTokens() const
{
	return (this->_tokens);
}

std::vector<Context>	Parser::getContextStack( void )
{
	return (this->_context_stack);
}

std::string	Token::getTypeStr( void ) const
{
	static std::map<TokenType, std::string>	type_map;
	
	if (type_map.empty() == true)
	{
		type_map[K_SERVER] = "K_SERVER";
		type_map[K_LOCATION] = "K_LOCATION";
		type_map[K_LISTEN] = "K_LISTEN";
		type_map[K_SERVERNAME] = "K_SERVERNAME";
		type_map[K_ROOT] = "K_ROOT";
		type_map[K_INDEX] = "K_INDEX";
		type_map[K_ERRORPAGE] = "K_ERRORPAGE";
		type_map[K_ALLOWEDMETHODS] = "K_ALLOWEDMETHODS";
		type_map[K_CLIENTMAXSIZEBODY] = "K_CLIENTMAXSIZEBODY";
		type_map[K_CGI] = "K_CGI";
		type_map[K_AUTOINDEX] = "K_AUTOINDEX";
		type_map[K_UPLOADALLOWED] = "K_UPLOADALLOWED";
		type_map[K_RETURN] = "K_RETURN";
		type_map[K_ON] = "K_ON";
		type_map[S_LBRACE] = "S_LBRACE";
		type_map[S_RBRACE] = "S_RBRACE";
		type_map[S_SEMICOLON] = "S_SEMICOLON";
		type_map[V_NUMBER] = "V_NUMBER";
		type_map[V_STR] = "V_STR";
		type_map[V_PATH] = "V_PATH";
		type_map[V_EXTENSION] = "V_EXTENSION";
		type_map[V_STATUSCODE] = "V_STATUSCODE";
		type_map[UNKNOW] = "UNKNOW";
	}
	return (type_map[this->_type]);
}

void	Parser::lowerStr(std::string& str)
{
	std::string::iterator it = str.begin();
	for (; it != str.end(); ++it)
		*it = std::tolower(*it);
}