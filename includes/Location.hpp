/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 02:46:45 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/16 02:33:15 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

# include <string>
# include <vector>
# include <sys/stat.h>

# include "ErrorPage.hpp"

/**************************\
 *	Class
\**************************/

class Location
{
	private:
		std::string					_root;
		std::string					_index;
		bool						_auto_index;
		bool						_upload_allowed;
		unsigned int				_return_code;
		std::string					_return_URI;
		std::vector<ErrorPage>		_error_page;
		std::vector<std::string>	_allowed_methods;
		std::vector<std::pair<std::string, std::string> >	_cgi_bin;

	public:
		Location() : _auto_index( false ), _upload_allowed( false ), _return_code( 0 ) {}
		~Location() {}

		void		setRoot( const std::string& root );
		void		setIndex( const std::string& index );
		void		setErrorPage( const std::vector<ErrorPage>& error_page );
		void		setAllowedMethods( const std::vector<std::string>& allowed_methods );
		void		setCgiBin( const std::vector< std::pair<std::string, std::string> >& cgi_bin );
		void		setAutoIndex( bool auto_index );
		void		setUploadAllowed( bool upload_allowed );
		void		setReturnCode( unsigned int return_code );
		void		setReturnURI( const std::string& return_URI );

		const std::string&					getRoot() const				{ return (_root); }
		const std::string&					getIndex() const			{ return (_index); }
		bool								getAutoIndex() const		{ return (_auto_index); }
		bool								getUploadAllowed() const	{ return (_upload_allowed); }
		unsigned int						getReturnCode() const		{ return (_return_code); }
		const std::string&					getReturnURI() const		{ return (_return_URI); }
		const std::vector<ErrorPage>&		getErrorPage() const;
		const std::vector<std::string>&		getAllowedMethods() const;
		const std::vector<std::pair<std::string, std::string> >&	getCgiBin() const;
};

//hierarchie des directives nginx