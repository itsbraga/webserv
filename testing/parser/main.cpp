#include "parser.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	Parser* parser = NULL;
	try {
		parser = new Parser(argv[1]);
		parser->bufferTokenize();
		std::cout << *parser << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	
}