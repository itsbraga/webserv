// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   Response.hpp                                       :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
// /*   Updated: 2025/08/23 19:43:52 by annabrag         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #pragma once

// # include "Message.hpp"

// class Response : public Message
// {
// 	private:
// 			int				_status_code; // verifier si pas unsigned int
// 			std::string		_status_name;
// 			std::string		_ressource_path;

// 			typedef void ( Response::*ResponseFunction )();
// 			static std::map<int, ResponseFunction>		_builders;
// 			static std::map<std::string, std::string>	_content_types;
	
// 	public:
// 			Response();
// 			Response( const int status_code, const std::string status_name );
// 			~Response();

// 			class ResourceForbiddenException : public std::exception {};
// 			class ResourceNotFoundException : public std::exception {};
// 			class InternalServerErrorException : public std::exception {};

// 			static void			initBuilders();
// 			static void			initContentTypes();

// 			void				process();

// 			void				setStatusCode( const int status_code );
// 			void				setStatusName( const std::string status_name );
// 			void				setRessourcePath( const std::string requested_ressource_path );
// 			void				setContentLength( const std::string length );
// 			void				setContentType( const std::string type );
// 			void				setDate();
// 			void				setLocation( const std::string location );
			
// 			const int&			getStatusCode() const;
// 			const std::string&	getStatusName() const;
// 			const std::string&	getRessourcePath() const;
// 			const std::string& 	getDate() const;
// 			const std::string&	getExtension( const std::string& URI ) const; 
// 			const std::string&	getSerializedHeaders() const;
// 			const std::string&	getSerializedResponse();

// 			void				defineContentType();

// 			/********************************************\
// 			 *	BUILDERS STATUS FUNCTIONS
// 			\********************************************/
			
// 			// 2xx : SUCCESS RESPONSE
// 			void	OK();						// 200
// 			void	Created();					// 201
// 			void	Accepted();					// 202
			
// 			// 3xx : REDIRECTION RESPONSE
// 			void	MovedPermanently(); 		// 301
			
// 			// 4xx : CLIENT ERROR RESPONSE
// 			void	BadRequest();				// 400
// 			void	Forbidden();				// 403
// 			void	NotFound();					// 404
// 			void	MethodNotAllowed();			// 405
// 			void	LengthRequired();			// 411
// 			void	URITooLong();				// 414
// 			void	ImATeapot();				// 418
// 			void	TooManyRequest();			// 429

// 			// 5xx : SERVER ERROR RESPONSE
// 			void	InternalServerError();		// 500
// 			void	NotImplemented();			// 501
// 			void	BadGateway();				// 502
// 			void	ServiceUnavailable();		// 503
// 			void	GatewayTimeout();			// 504
// 			void	HttpVersionNotSupported();	// 505
// };

// inline std::ostream		&operator<<( std::ostream& os, Response const& response );