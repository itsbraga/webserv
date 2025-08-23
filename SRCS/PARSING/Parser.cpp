/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/23 20:03:15 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	Parser::handleFileConfig(char *arg, webserv_s *data)
{
	Parser* parser = NULL;
	try
	{
		parser = new Parser(arg);
		parser->bufferTokenize();
	}
	catch (const std::exception &)
	{
		if (parser != NULL)
			delete parser;
		throw;
	}
}

Parser::Parser(char *arg)
{
	try
	{
		this->_conf_path = this->checkPath(arg);
	}
	catch (std::exception &)
	{
		throw;
	}
	std::ifstream file(this->_conf_path);
	if (!file.is_open())
		throw std::runtime_error("Creation of an ifstream failed");
	std::ostringstream ss;
	ss << file.rdbuf();
	if (file.bad() == true)
		std::runtime_error("Rdbuf function failed");
	this->_buffer = ss.str();
	file.close();
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
	std::string::iterator	beg_tok = this->_buffer.begin();
	std::string::iterator	end_tok = beg_tok;
	
	for (beg_tok; beg_tok != this->_buffer.end(); beg_tok++)
	{
		if 
	}
}

std::stringstream	Parser::createStringStream( void )
{
	std::stringstream ss(this->_buffer);
	return (ss);
}

void	Parser::initKeywordSet( void )
{
	this->_keywords.insert("server");
	this->_keywords.insert("location");
	this->_keywords.insert("listen");
	this->_keywords.insert("server_name");
	this->_keywords.insert("root");
	this->_keywords.insert("index");
	this->_keywords.insert("error_page");
	this->_keywords.insert("allowed_methods");
	this->_keywords.insert("cgi");
	this->_keywords.insert("autoindex");
	this->_keywords.insert("upload_allowed");
	this->_keywords.insert("return");
	this->_keywords.insert("on");
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
}

std::string	Parser::getConfPath( void )
{
	return (this->_conf_path);
}

std::string	Parser::getBuffer( void )
{
	return (this->_buffer);
}

std::vector<Context>	Parser::getContextStack( void )
{
	return (this->_context_stack);
}
