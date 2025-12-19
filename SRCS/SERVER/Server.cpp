#include "webserv.hpp"

Server::Server()
{
	this->_port = 0;
	this->_auto_index = false;
	this->_return_code = 0;
}

void	Server::setPort( unsigned short int port ) {this->_port = port;}
void	Server::setServerName( const std::string server_name ) {this->_server_name = server_name;}
void	Server::setRoot( const std::string root ) {this->_root = root;}
void	Server::setIndex( const std::string index ) {this->_index = index;}
void	Server::setClientMaxSizeBody( const std::string max_size ) {this->_client_max_size_body = max_size;}
void	Server::setAutoIndex( const bool auto_index ) {this->_auto_index = auto_index;}
void	Server::setReturnCode( const unsigned int return_code ) {this->_return_code = return_code;}
void	Server::setReturnUri( const std::string return_uri ) {this->_return_uri = return_uri;}

const unsigned short int&				Server::getPort() const {return this->_port;}
const std::string&						Server::getServerName() const {return this->_server_name;}
const std::string&						Server::getRoot() const {return this->_root;}
const std::string& 						Server::getIndex() const {return this->_index;}
const std::vector<ErrorPage>&			Server::getErrorPage() const {return this->_epage;}
const std::string&						Server::getClientMaxSizeBody() const {return this->_client_max_size_body;}
const bool&								Server::getAutoIndex() const {return this->_auto_index;}
const unsigned int&						Server::getReturnCode() const {return this->_return_code;}
const std::string&						Server::getReturnUri() const {return this->_return_uri;}
const std::map<std::string, Location>& 	Server::getLocations() const {return this->_locations;}

unsigned short int&						Server::getPort() {return this->_port;}
std::string&							Server::getServerName() {return this->_server_name;}
std::string&							Server::getRoot() {return this->_root;}
std::string& 							Server::getIndex() {return this->_index;}
std::vector<ErrorPage>&					Server::getErrorPage() {return this->_epage;}
std::string&							Server::getClientMaxSizeBody() {return this->_client_max_size_body;}
bool&									Server::getAutoIndex() {return this->_auto_index;}
unsigned int&							Server::getReturnCode() {return this->_return_code;}
std::string&							Server::getReturnUri() {return this->_return_uri;}
std::map<std::string, Location>& 		Server::getLocations() {return this->_locations;}