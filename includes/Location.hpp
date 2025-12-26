/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 02:46:45 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/26 17:38:34 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Class
\**************************/

class Location
{
	private:
		std::string					_uri;
		std::string					_root;
		std::string 				_index;
		std::vector<ErrorPage>		_err_page;
		std::vector<std::string>	_allowed_methods;
		std::vector<std::string>	_cgi_extension;
		bool						_auto_index;
		std::string					_client_max_body_size;
		bool						_upload_allowed;
		std::string					_upload_path;
		unsigned int				_return_code;
		std::string					_return_uri;

	public:
		Location();
		~Location();
		Location&	operator=( const Location& toCopy );

		void		setUri( const std::string& uri );
		void		setRoot( const std::string& root );
		void		setIndex( const std::string& index );
		void		setAllowedMethods( const std::vector<std::string>& allowed_methods );
		void		setCgiExtension( const std::vector<std::string>& cgi_extension );
		void		setClientMaxSizeBody( const std::string& max_size );
		void		setAutoIndex( bool auto_index );
		void		setUploadAllowed( bool upload_allowed );
		void		setUploadPath( const std::string& path );
		void		setReturnCode( unsigned int return_code );
		void		setReturnUri( const std::string& return_uri );
		
		std::string&												getUri()						{ return (_uri); }
		std::string&												getRoot()						{ return (_root); }
		std::string& 												getIndex()						{ return (_index); }
		std::vector<ErrorPage>&										getErrorPage()					{ return (_err_page); }
		std::vector<std::string>&									getAllowedMethods()				{ return (_allowed_methods); }
		std::vector<std::string>&									getCgiExtension()				{ return (_cgi_extension); }
		std::string&												getClientMaxSizeBody()			{ return (_client_max_body_size); }
		bool&														getAutoIndex()					{ return (_auto_index); }
		bool&														getUploadAllowed()				{ return (_upload_allowed); }
		std::string&												getUploadPath()					{ return (_upload_path); }
		unsigned int&												getReturnCode()					{ return (_return_code); }
		std::string&												getReturnUri()					{ return (_return_uri); }

		const std::string&											getUri() const					{ return (_uri); }
		const std::string&											getRoot() const					{ return (_root); }
		const std::string& 											getIndex() const				{ return (_index); }
		const std::vector<ErrorPage>&								getErrorPage() const			{ return (_err_page); }
		const std::vector<std::string>&								getAllowedMethods() const		{ return (_allowed_methods); }
		const std::vector<std::string>&								getCgiExtension() const			{ return (_cgi_extension); }
		const std::string&											getClientMaxSizeBody() const	{ return (_client_max_body_size); }
		bool														getAutoIndex() const			{ return (_auto_index); }
		bool														getUploadAllowed() const		{ return (_upload_allowed); }
		const std::string&											getUploadPath() const			{ return (_upload_path); }
		unsigned int												getReturnCode() const			{ return (_return_code); }
		const std::string&											getReturnUri() const			{ return (_return_uri); }
	};