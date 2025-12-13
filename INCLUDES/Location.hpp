/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 02:46:45 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/13 23:49:40 by panther          ###   ########.fr       */
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
		ErrorPage( std::vector<int> status, std::string file ) : _status( status ), _file( file ) {}
		~ErrorPage() {}
		
		const std::vector<int>&		getStatus() const	{ return (_status); }
		const std::string&			getFile() const		{ return (_file); }

		bool		findStatus( int target ) const;
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

		void		setRoot( std::string root );
		void		setIndex( std::string index );
		void		setErrorPage( std::vector<ErrorPage> error_page );
		void		setAllowedMethods( std::vector<std::string> allowed_methods );
		void		setCgiBin( std::vector<std::pair<std::string, std::string> > cgi_bin );
		void		setAutoIndex( bool auto_index );
		void		setUploadAllowed( bool upload_allowed );
		void		setReturnCode( unsigned int return_code );
		void		setReturnUri( std::string return_uri );

		std::string&										getRoot() const;
		std::string&										getIndex() const;
		std::vector<ErrorPage>&								getErrorPage() const;
		std::vector<std::string>&							getAllowedMethods() const;
		std::vector<std::pair<std::string, std::string> >&	getCgiBin() const;
		bool&												getAutoIndex() const;
		bool&												getUploadAllowed() const;
		unsigned int&										getReturnCode() const;
		std::string&										getReturnUri() const;	
};

//hierarchie des directives nginx