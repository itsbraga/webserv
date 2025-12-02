#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

/**************************\
 *	Modes
\**************************/

# define RESET			"\e[0m"
# define BOLD			"\e[1m"
# define ITAL			"\e[3m"
# define UNDERLINE		"\e[4m"

/**************************\
 *	Standard colors
\**************************/

# define BLACK			"\e[30m"
# define GRAY			"\e[90m"
# define RED			"\e[31m"
# define GREEN			"\e[32m"
# define YELLOW			"\e[33m"
# define SAND			"\e[38;2;231;200;160m"
# define DARK_SAND		"\e[38;2;212;163;115m"
# define ORANGE			"\e[38;5;208m"
# define BLUE			"\e[34m"
# define PURPLE			"\e[35m"
# define HOT_PINK		"\e[38;2;255;105;180m"
# define PINK			"\e[38;2;255;182;193m"
# define CYAN			"\e[36m"
# define WHITE			"\e[37m"

# define L_GRAY			"\e[38;2;173;165;165m"
# define L_GRAY2		"\e[38;2;211;211;211m"

class Parser
{
	private:
		std::string _buffer;
		std::vector<std::string> _tokens;
	public:
		Parser(char *arg);
		~Parser();

		std::string	checkPath(char *arg);
		void		bufferTokenize( void );

		void						fillBuffer(const std::ifstream &infile);
		void						createTokenDelimiter(std::string::const_iterator it);

		std::string&				getBuffer();
		std::vector<std::string>&	getTokens();
		const std::vector<std::string>&	getTokens() const;

		bool	isSemiColon(char c) const;
		bool	isLeftBrace(char c) const;
		bool	isRightBrace(char c) const;
		bool	isWhiteSpace(char c) const;

};

inline std::ostream&	operator<<(std::ostream &os, const Parser& to_insert)
{
	std::vector<std::string>::const_iterator it = to_insert.getTokens().begin();
	for (; it != to_insert.getTokens().end(); ++it)
	{
		os << RED << "[" << RESET << *it << RED << "]" << RESET << " - ";
	}
	os << std::endl;
	return (os);
}