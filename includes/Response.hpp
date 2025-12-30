/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/30 17:22:40 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Class
\**************************/

class Response : public Message
{
	private:
		int				_status_code;
		std::string		_status_name;
		std::string		_resource_path;

		typedef void ( Response::*ResponseFunction )();

		static std::map<int, ResponseFunction>		_builders;
		static std::map<std::string, std::string>	_content_types;

		void			_setCommonHeaders( bool keepAlive );
		void			_setDefaultErrorPage( const std::string& title );
	
	public:
		Response( const int status_code, const std::string& status_name );
		~Response();

		void			process();
		
		void			setStatusCode( const int status_code );
		void			setStatusName( const std::string& status_name );
		void			setResourcePath( const std::string& requested_resource_path );
		void			setContentLength( const std::string& length );
		void			setContentType( const std::string& type );
		void			setDate();
		void			setLocation( const std::string& location );

		void			setGeneratedContent( const std::string& body, const std::string& mime_type );
		void			setGeneratedHeaders( const std::string& body, const std::string& mime_type );
		void			defineContentType();
		void			setFileContent( const std::string& body, const std::string& file_path );
		void			setFileHeaders( const std::string& body, const std::string& file_path );		

		int					getStatusCode() const		{ return (_status_code); }
		const std::string&	getStatusName() const		{ return (_status_name); }
		const std::string&	getResourcePath() const		{ return (_resource_path); }
		const std::string 	getDate() const;
		const std::string	getExtension( const std::string& uri ) const; 
		const std::string	getSerializedHeaders() const;
		const std::string	getSerializedResponse();
		
		static std::string	getStatusNameFromStatusCode( int status_code );

		static void		initBuilders();
		static void		initContentTypes();

		void			replaceBody( const std::string& body, const std::string& mime_type );

		/********************************************\
		 *	Builders Status Functions
		\********************************************/
		
		// 2xx : Successful
		void	OK();						// 200
		void	Created();					// 201
		void	NoContent();				// 204
		
		// 3xx : Redirection
		void	MovedPermanently(); 		// 301
		void	Found();					// 302
		void	TemporaryRedirect();		// 307
		void	PermanentRedirect();		// 308
		
		// 4xx : Client error
		void	BadRequest();				// 400
		void	Forbidden();				// 403
		void	NotFound();					// 404
		void	MethodNotAllowed();			// 405
		void	Gone();						// 410
		void	LengthRequired();			// 411
		void	PayloadTooLarge();			// 413
		void	URITooLong();				// 414
		void	ImATeapot();				// 418
		void	TooManyRequest();			// 429

		// 5xx : Server error
		void	InternalServerError();		// 500
		void	NotImplemented();			// 501
		void	BadGateway();				// 502
		void	ServiceUnavailable();		// 503
		void	GatewayTimeout();			// 504
		void	HttpVersionNotSupported();	// 505
};