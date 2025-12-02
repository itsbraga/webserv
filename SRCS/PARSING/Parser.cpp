/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 19:30:25 by pmateo           ###   ########.fr       */
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
			&& !isSemiColon(*end_tok)
			&& !isLeftBrace(*end_tok)
			&& !isRightBrace(*end_tok)
			&& !isWhiteSpace(*end_tok))
			end_tok++;
		std::string	token_value(start_tok, end_tok);
		if (start_tok != end_tok)
		{
			
			this->_tokens.push_back(token);
		}
		if (end_tok != this->_buffer.end())
		{
			if ((isSemiColon(*end_tok)) || isLeftBrace(*end_tok) || isRightBrace(*end_tok))
				createTokenDelimiter(end_tok);
		}
		start_tok = end_tok;
		if (start_tok != this->_buffer.end())
			++start_tok;
	}
}

Token		Parser::createToken(std::string value) const
{
	
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
	else
		return (V_PATH);
}

void		Parser::createTokenDelimiter(std::string::const_iterator it)
{
	std::string token(it, it + 1);
	this->_tokens.push_back(token);
}

void		Parser::fillBuffer(const std::ifstream& infile)
{
	std::ostringstream	buffer;
	buffer << infile.rdbuf();
	this->_buffer = buffer.str();
}

std::stringstream	Parser::createStringStream( void )
{
	std::stringstream ss(this->_buffer);
	return (ss);
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
	this->_keywords["cgi"] = K_CGI;
	this->_keywords["autoindex"] = K_AUTOINDEX;
	this->_keywords["upload_allowed"] = K_UPLOADALLOWED;
	this->_keywords["return"] = K_RETURN;
	this->_keywords["on"] = K_ON;
}

bool	Parser::isLeftBrace(char c)
{
	return (c == '{');
}

bool	Parser::isRightBrace(char c)
{
	return (c == '}');
}

bool	Parser::isSemiColon(char c)
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

bool	Parser::isKeyword(const std::string& to_compare)
{
	return (this->_keywords.find(to_compare) != this->_keywords.end());
}

bool	Parser::isNumber(const std::string& to_compare)
{
	std::string::const_iterator it;
	
	for (it = to_compare.begin(); it != to_compare.end(); it++)
	{
		if (isdigit(static_cast<int>(*it)) == 0)
			return(false);
	}
	return (true);
}

bool	Parser::isServer(const std::string& to_compare)
{
	return (to_compare == "server");
}

bool	Parser::isLocation(const std::string& to_compare)
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
	//all path doesn't always return
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
