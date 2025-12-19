#include "webserv.hpp"

Server::Server()
{
	this->_port = 0;
	this->_auto_index = false;
	this->_return_code = 0;
}