/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/19 22:52:14 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "webserv.hpp"

std::vector<int>&	ErrorPage::getStatus() {return (this->_status);}

const std::vector<int>&	ErrorPage::getStatus() const {return (this->_status);}

const std::string&	ErrorPage::getFile() const {return(this->_file);}

void	ErrorPage::setFile(std::string file) {this->_file = file;}

bool ErrorPage::findStatus(int target)
{
	if (*this->_status.begin() == target)
		return (true);
	else
	{
		std::vector<int>::iterator it;
		for(it = this->_status.begin(); it != this->_status.end(); it++)
		{
			if (*it == target)
				return (true);
		}
		return(false);
	}
}

void					Location::setUri(const std::string uri) {this->_uri = uri;}
void					Location::setRoot(std::string root) {this->_root = root;}
void					Location::setIndex(std::string index) {this->_index = index;}
void					Location::setAllowedMethods(std::vector<std::string> allowed_methods) {this->_allowed_methods = allowed_methods;}
void					Location::setClientMaxSizeBody(std::string max_size) {this->_client_max_size_body = max_size;}
void					Location::setAutoIndex(bool auto_index) {this->_auto_index = auto_index;}
void					Location::setUploadAllowed(bool upload_allowed) {this->_upload_allowed = upload_allowed;}
void					Location::setUploadPath(const std::string path) {this->_upload_path = path;}
void					Location::setReturnCode(unsigned int return_code) {this->_return_code = return_code;}
void					Location::setReturnUri(std::string return_uri) {this->_return_uri = return_uri;}

const std::string&											Location::getUri() const {return this->_uri;}
const std::string&											Location::getRoot() const {return this->_root;}
const std::string& 											Location::getIndex() const {return this->_index;}
const std::vector<ErrorPage>&								Location::getErrorPage() const {return this->_epage;}
const std::vector<std::string>&								Location::getAllowedMethods() const {return this->_allowed_methods;}
const std::vector<std::pair<std::string, std::string> >&	Location::getCgiBin() const {return this->_cgi_bin;}
const std::string&											Location::getClientMaxSizeBody() const {return this->_client_max_size_body;}
const bool&													Location::getAutoIndex() const {return this->_auto_index;}
const bool&													Location::getUploadAllowed() const {return this->_upload_allowed;}
const std::string&											Location::getUploadPath() const {return this->_upload_path;}
const unsigned int&											Location::getReturnCode() const {return this->_return_code;}
const std::string&											Location::getReturnUri() const {return this->_return_uri;}

std::string&											Location::getUri() {return this->_uri;}
std::string&											Location::getRoot() {return this->_root;}
std::string& 											Location::getIndex() {return this->_index;}
std::vector<ErrorPage>&									Location::getErrorPage() {return this->_epage;}
std::vector<std::string>&								Location::getAllowedMethods() {return this->_allowed_methods;}
std::vector<std::pair<std::string, std::string> >&		Location::getCgiBin() {return this->_cgi_bin;}
std::string&											Location::getClientMaxSizeBody() {return this->_client_max_size_body;}
bool&													Location::getAutoIndex() {return this->_auto_index;}
bool&													Location::getUploadAllowed() {return this->_upload_allowed;}
std::string&											Location::getUploadPath() {return this->_upload_path;}
unsigned int&											Location::getReturnCode() {return this->_return_code;}
std::string&											Location::getReturnUri() {return this->_return_uri;}