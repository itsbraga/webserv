/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:39:18 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/23 18:13:14 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "webserv.hpp"

class Message
{		 
	protected:
			Message();

			const std::string									_http_version;
			std::vector< std::pair<std::string, std::string> >	_headers;
			std::string											_body;
	
	public:
			virtual ~Message() {}

			virtual void	process() = 0;

			void				setHeaderValue( const std::string key, const std::string value );
			void				addHeader( const std::string first, const std::string second );
			void				setBody( const std::string body );
			const std::string&	getHttpVersion() const;
			const std::string&	getHeaderMap() const;
			const std::string&	getHeaderValue( const std::string& key ) const;
			const std::string&	getBody() const;
};