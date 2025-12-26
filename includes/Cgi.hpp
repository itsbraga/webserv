/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:55:12 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/26 18:01:15 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Request;
class ServerConfig;
class Location;

/**************************\
 *	Functions
\**************************/

// Cgi.cpp
Response*		doCgi( const Request& request, const ServerConfig& server, const std::string& path );
void			childExec( const Request& request, const ServerConfig& server, const std::string& path, int (&pipes)[2][2] );
char**			createEnvp( const Request& request, const ServerConfig& server, const std::string& path );
Response*		handleOutput( const std::string& output );
std::multimap<std::string, std::string>		handleOutputHeaders( const std::string& headers_str );
std::string		toUpperCgi( const std::string& name );
bool			isCgiRequest(const Request& request, const ServerConfig& server);

Response*		cgiHandler( const Request& request, const ServerConfig& server );
