/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:37:42 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/03 17:43:09 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "webserv.hpp"

/*
	----------------------------- [ Helper ] -----------------------------
*/
std::string		Parser::_checkPath( char *arg)
{
	std::string path = arg;
	if (path.empty())
		throw std::invalid_argument( "No file path" );
	
	struct stat buff;
	if (stat(arg, &buff) == -1)
		throw std::invalid_argument( strerror(errno) );
	else
	{
		if (S_ISDIR(buff.st_mode))
			throw std::invalid_argument( "Is a directory" );
	}
	if (path.substr(path.find_last_of('.')) != ".conf")
		throw std::invalid_argument( "Invalid file extension" );
	return (path);
}

/*
	---------------------- [ Object Manipulation ] -----------------------
*/
Parser::Parser( char *arg )
{
	try {
		_conf_path = this->_checkPath(arg);
	}
	catch (std::exception &) {
		throw ;
	}

	std::ifstream file(_conf_path);
	if (!file.is_open())
		throw std::runtime_error( "Creation of an ifstream failed" );
	std::ostringstream ss;
	ss << file.rdbuf();
	if (file.bad() == true)
		std::runtime_error( "Rdbuf function failed" );
	_buffer = ss.str();
	file.close();
}

/*
	---------------------------- [ Methods ] -----------------------------
*/
void	Parser::handleFileConfig( char *arg, webserv_s* data )
{
	Parser* parser = NULL;

	try {
		parser = new Parser(arg);
		parser->bufferTokenize();
	}
	catch (const std::exception &) {
		if (parser != NULL)
			delete parser;
		throw ;
	}
}

void	Parser::bufferTokenize()
{
	std::string::iterator	beg_tok = _buffer.begin();
	std::string::iterator	end_tok = beg_tok;
	
	// for (beg_tok; beg_tok != _buffer.end(); beg_tok++)
	// {
	// 	if 
	// }
}

std::stringstream	Parser::createStringStream()
{
	std::stringstream ss(_buffer);
	return (ss);
}

void	Parser::initKeywordSet()
{
	_keywords.insert("server");
	_keywords.insert("location");
	_keywords.insert("listen");
	_keywords.insert("server_name");
	_keywords.insert("root");
	_keywords.insert("index");
	_keywords.insert("error_page");
	_keywords.insert("allowed_methods");
	_keywords.insert("cgi");
	_keywords.insert("autoindex");
	_keywords.insert("upload_allowed");
	_keywords.insert("return");
	_keywords.insert("on");
}

bool	Parser::isKeyword( const std::string& to_compare )
{
	return (_keywords.find(to_compare) != _keywords.end());
}

bool	Parser::isNumber( const std::string& to_compare )
{
	std::string::const_iterator it;
	for (it = to_compare.begin(); it != to_compare.end(); it++)
	{
		if (isdigit(static_cast<int>(*it)) == 0)
			return(false);
	}
	return (true);
}

bool	Parser::isServer( const std::string& to_compare )
{
	return (to_compare == "server");
}

bool	Parser::isLocation( const std::string& to_compare )
{
	return (to_compare == "location");
}

void	Parser::enterContext( Context context )
{
	_context_stack.push_back(context);
}

void	Parser::exitContext()
{
	if (_context_stack.empty() == false)
		_context_stack.pop_back();
}

Context		Parser::getCurrentContext()
{
	if (_context_stack.empty() == false)
		return (_context_stack.back());
	// all path doesn't always return
}