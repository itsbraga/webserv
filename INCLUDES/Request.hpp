/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:42:27 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/21 20:22:41 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Message.hpp"

class Request : public Message
{
	private:
			std::string	_method;
			std::string	_URI;
	
	public:
			Request();
			Request( const std::string& serialized );
			~Request();

			void	deserializeRequest( const std::string& serialized );
};