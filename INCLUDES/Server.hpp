/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 17:16:44 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Server
{
	private :
		unsigned short int 	_port;
		std::string 		_server_name;
		LocationTree*		_tree;
		
	public : 
		Server( unsigned short int port, std::string server_name ) : _port(port), _server_name(server_name) {}
		~Server() {}
};