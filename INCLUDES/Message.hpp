/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:39:18 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/17 21:16:21 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Webserv.hpp"

class Message
{		 
	protected:
			Message();

			const std::string	_http_version;
			std::vector< std::pair< std::string, std::string > > _headers;
			std::string			_body;
	
	public:
			virtual ~Message() {}

			virtual void	process() = 0;

			void				setHeaderMap( const std::string headermap );
			void				setHeaderValue( const std::string key, const std::string value );
			void				addHeader( const std::string first, const std::string second );
			void				setBody( const std::string body );
			const std::string&	getHttpVersion() const;
			std::string			getHeaderMap() const;
			std::string			getHeaderValue( std::string key ) const;
			std::string			getBody() const;
};