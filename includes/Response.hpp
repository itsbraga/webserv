/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/23 20:47:12 by annabrag         ###   ########.fr       */
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
		void			_setErrorPage( const std::string& title );
	
	public:
		Response( const int status_code, const std::string& status_name );
		~Response() {}

		static void			initBuilders();
		static void			initContentTypes();

		void				process();

		void				setStatusCode( const int status_code );
		void				setStatusName( const std::string& status_name );
		void				setResourcePath( const std::string& requested_resource_path );
		void				setContentLength( const std::string& length );
		void				setContentType( const std::string& type );
		void				setDate();
		void				setLocation( const std::string& location );
		
		int					getStatusCode() const		{ return (_status_code); }
		const std::string&	getStatusName() const		{ return (_status_name); }
		const std::string&	getResourcePath() const		{ return (_resource_path); }
		const std::string 	getDate() const;
		const std::string	getExtension( const std::string& uri ) const; 
		const std::string	getSerializedHeaders() const;
		const std::string	getSerializedResponse();

		void				defineContentType();
		void				loadContent( const std::string& body, const std::string& path );
		void				loadHeaders( const std::string& body, const std::string& path );

		/********************************************\
		 *	Builders Status Functions
		\********************************************/
		
		// 2xx : Successful
		void	OK();						// 200
		void	Created();					// 201
		void	Accepted();					// 202
		
		// 3xx : Redirection
		void	MovedPermanently(); 		// 301
		void	Found();					// 302
		
		// 4xx : Client error
		void	BadRequest();				// 400
		void	Forbidden();				// 403
		void	NotFound();					// 404
		void	MethodNotAllowed();			// 405
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