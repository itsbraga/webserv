/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 02:46:45 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/19 03:36:34 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class ErrorPage
{
	private:
		std::vector<int>	_status;
		std::string 		_file;
		
	public: 
		ErrorPage(std::vector<int> status, std::string file) : _status(status), _file(file) {}
	 	~ErrorPage(){}
		
		std::vector<int>	getStatus();
		std::string 		getFile();

		bool		findStatus(int target);
};

class Location
{
	private:
		std::string							_uri;
		std::string 						_root;
		std::string 							_index;
		std::vector<ErrorPage>						 _epage;
		std::vector<std::string> 						_allowed_methods;
		std::vector<std::pair<std::string, std::string>>	_cgi_bin; //first == extension; second == path_to_script;
		std::string										_client_max_size_body;
		bool											_auto_index;
		bool										_upload_allowed;
		std::string									_upload_path;
		unsigned int							_return_code;
		std::string							_return_uri;

	public:
		Location(){}
		~Location(){}

		void					setUri(const std::string uri);
		void					setRoot(std::string root);
		void					setIndex(std::string index);
		void					setErrorPage(std::vector<ErrorPage> error_page);
		void					setAllowedMethods(std::vector<std::string> allowed_methods);
		void					setCgiBin(std::vector<std::pair<std::string, std::string>> cgi_bin);
		void					setClientMaxSizeBody(std::string max_size);
		void					setAutoIndex(bool auto_index);
		void					setUploadAllowed(bool upload_allowed);
		void					setUploadPath(const std::string path);
		void					setReturnCode(unsigned int return_code);
		void					setReturnUri(std::string return_uri);
		std::string											getUri() const;
		std::string											getRoot();
		std::string 										getIndex();
		std::vector<ErrorPage>								getErrorPage();
		std::vector<std::string>							getAllowedMethods();
		std::vector<std::pair<std::string, std::string>>	getCgiBin();
		std::string											getClientMaxSizeBody(std::string max_size);
		bool					getAutoIndex();
		bool					getUploadAllowed();
		std::string				getUploadPath() const;
		unsigned int			getReturnCode();
		std::string				getReturnUri();

		
};

//hierarchie des directives nginx