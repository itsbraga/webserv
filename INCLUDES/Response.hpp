/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/11 23:28:26 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Libraries
\**************************/

# include <iostream>
# include <string>
# include <map>
# include <ctime>
# include <sys/stat.h>

# include "Message.hpp"
# include "utils.hpp"
# include "colors.hpp"

/**************************\
 *	Class
\**************************/

class Response : public Message
{
	private:
		int				_status_code; // verifier si pas unsigned int
		std::string		_status_name;
		std::string		_ressource_path;

		typedef void ( Response::*ResponseFunction )();

		static std::map<int, ResponseFunction>		_builders;
		static std::map<std::string, std::string>	_content_types;
	
	public:
		Response( const int status_code, const std::string status_name );
		~Response() {}

		static void			initBuilders();
		static void			initContentTypes();

		void				process();

		void				setStatusCode( const int status_code );
		void				setStatusName( const std::string status_name );
		void				setRessourcePath( const std::string requested_ressource_path );
		void				setContentLength( const std::string length );
		void				setContentType( const std::string type );
		void				setDate();
		void				setLocation( const std::string location );
		
		const int&			getStatusCode() const		{ return (_status_code); }
		const std::string&	getStatusName() const		{ return (_status_name); }
		const std::string&	getRessourcePath() const	{ return (_ressource_path); }
		const std::string 	getDate() const;
		const std::string	getExtension( const std::string& URI ) const; 
		const std::string	getSerializedHeaders() const;
		const std::string	getSerializedResponse();

		void				defineContentType();

		/********************************************\
		 *	Builders Status Functions
		\********************************************/
		
		// 2xx : SUCCESS RESPONSE
		void	OK();						// 200
		void	Created();					// 201
		void	Accepted();					// 202
		
		// 3xx : REDIRECTION RESPONSE
		void	MovedPermanently(); 		// 301
		
		// 4xx : CLIENT ERROR RESPONSE
		void	BadRequest();				// 400
		void	Forbidden();				// 403
		void	NotFound();					// 404
		void	MethodNotAllowed();			// 405
		void	LengthRequired();			// 411
		void	URITooLong();				// 414
		void	ImATeapot();				// 418
		void	TooManyRequest();			// 429

		// 5xx : SERVER ERROR RESPONSE
		void	InternalServerError();		// 500
		void	NotImplemented();			// 501
		void	BadGateway();				// 502
		void	ServiceUnavailable();		// 503
		void	GatewayTimeout();			// 504
		void	HttpVersionNotSupported();	// 505

		class RessourceForbiddenException : public std::exception
		{
			const char	*what() const throw();
		};
		class RessourceNotFoundException : public std::exception
		{
			const char	*what() const throw();
		};
		class InternalServerErrorException : public std::exception
		{
			const char	*what() const throw();
		};
};

inline std::ostream		&operator<<( std::ostream& os, Response const& response );