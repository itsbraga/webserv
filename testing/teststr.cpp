#include <string>
#include <iostream>
#include <cstdlib>

void	erase_whitespace( std::string& str, std::string set )
{
	std::string::const_iterator set_it = set.begin();
	std::string::iterator 		str_it = str.begin();
	size_t						pos = std::string::npos;

	while(set_it != set.end())
	{
		pos = str.find(*set_it);
		if (pos != std::string::npos)
		{
			str.erase(str_it + pos);
			pos = std::string::npos;
		}
		else
			set_it++;
	}
}

int main( void )
{
	std::string str = "               Server         { listen 8080; host localhost;\n Location /uploads\n {\n\tautonindex on;\n\tallow_uploads;\n   }           }";
	std::cout << str << std::endl;
	erase_whitespace(str, "\t\v\r\n");
	std::cout << "################################################" << std::endl;
	std::cout << str << std::endl;
}