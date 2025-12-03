/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 02:46:45 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/03 18:06:26 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <string>
# include <vector>
# include <sys/stat.h>

/**************************\
 *	Classes
\**************************/

class ErrorPage
{
	private:
		std::vector<int>	_status;
		std::string 		_file;
		
	public: 
		ErrorPage( std::vector<int> status, std::string file ) : _status(status), _file(file) {}
	 	~ErrorPage() {}
		
		std::vector<int>	getStatus()	{ return (_status); }
		std::string 		getFile()	{ return (_file); }

		bool	findStatus( int target );
};

class Location
{
	private:
		std::string					_root;
		std::string					_index;
		std::vector<ErrorPage>		_epage;
		std::vector<std::string>	_allowed_methods;
		std::vector<std::pair<std::string, std::string> >	_cgi_bin;
		bool						_auto_index;
		bool						_upload_allowed;
		unsigned int				_return_code;
		std::string					_return_uri;

	public:
		Location() {}
		~Location() {}

		void	setRoot( std::string root );
		void	setIndex( std::string index );
		void	setErrorPage( std::vector<ErrorPage> error_page );
		void	setAllowedMethods( std::vector<std::string> allowed_methods );
		void	setCgiBin( std::vector<std::pair<std::string, std::string>> cgi_bin );
		void	setAutoIndex( bool auto_index );
		void	setUploadAllowed( bool upload_allowed );
		void	setReturnCode( unsigned int return_code );
		void	setReturnUri( std::string return_uri );

		std::string											getRoot();
		std::string 										getIndex();
		std::vector<ErrorPage>								getErrorPage();
		std::vector<std::string>							getAllowedMethods();
		std::vector<std::pair<std::string, std::string>>	getCgiBin();
		bool												getAutoIndex();
		bool												getUploadAllowed();
		unsigned int										getReturnCode();
		std::string											getReturnUri();	
};

//hierarchie des directives nginx