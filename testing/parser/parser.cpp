#include "parser.hpp"

Parser::Parser( char *arg )
{
	try {
		this->checkPath(arg);
		std::ifstream infile;
		infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		infile.open(arg);
		if (infile.peek() == EOF)
			throw std::invalid_argument( "Configuration file is empty !" );
		this->fillBuffer(infile);
	}
	catch (const std::exception& e) {
		throw ;
	}
	
}

Parser::~Parser() {}

std::string		Parser::checkPath( char *arg )
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

void	Parser::bufferTokenize()
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
		std::string	token(start_tok, end_tok);
		if (start_tok != end_tok)
			this->_tokens.push_back(token);
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

void	Parser::createTokenDelimiter( std::string::const_iterator it )
{
	std::string token(it, it + 1);
	this->_tokens.push_back(token);
}

void	Parser::fillBuffer( const std::ifstream& infile )
{
	std::ostringstream	buffer;
	buffer << infile.rdbuf();
	this->_buffer = buffer.str();
}

std::string&	Parser::getBuffer()
{
	return (this->_buffer);
}

std::vector<std::string>&	Parser::getTokens()
{
	return (this->_tokens);
}

const std::vector<std::string>&		Parser::getTokens() const
{
	return (this->_tokens);
}

bool	Parser::isSemiColon( char c ) const
{
	return (c == ';');
}

bool	Parser::isLeftBrace( char c ) const
{
	return (c == '{');
}

bool	Parser::isRightBrace( char c ) const
{
	return (c == '}');
}

bool	Parser::isWhiteSpace( char c ) const
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		return (true);
	else
		return (false);
}