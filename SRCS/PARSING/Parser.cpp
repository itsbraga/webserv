/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/14 02:55:17 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCLUDES/Parser.hpp"

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
	else if (isRightBrace(to_identify[0] == true))
		return (S_RBRACE);
	else
		return (S_SEMICOLON);
}

TokenType	Parser::identifyValue(const std::string& to_identify) const
{
	if (isNumber(to_identify) == true)
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
	return (isNumber(to_compare) || isString(to_compare) || isPath(to_compare) || isExtension(to_compare));
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

	for (it = to_compare.begin(); it != to_compare.end(); it++)
	{
		if (isalpha(static_cast<int>(*it)) == 0)
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

bool	Parser::isServer(const std::string& to_compare) const
{
	return (to_compare == "server");
}

bool	Parser::isLocation(const std::string& to_compare) const
{
	return (to_compare == "location");
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

Context	Parser::getCurrentContext( void )
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