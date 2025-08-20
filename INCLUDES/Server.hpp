/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:18:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/20 18:20:01 by pmateo           ###   ########.fr       */
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
		Server(unsigned short int port, std::string server_name) 
		: _port(port), _server_name(server_name) {}
		~Server(){}

};