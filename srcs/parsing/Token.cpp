/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:24:35 by annabrag          #+#    #+#             */
/*   Updated: 2025/12/23 18:29:36 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	----------------------------- [ Setters ] ----------------------------
*/
void	Token::setType( TokenType type )
{
	_type = type;
}

void	Token::setValue( std::string value )
{
	if (value.empty())
		return ;

	_value = value;
}

/*
	----------------------------- [ Getter ] -----------------------------
*/
std::string		Token::getTypeStr() const
{
	static std::map<TokenType, std::string>	type_map;
	
	if (type_map.empty())
	{
		type_map[K_SERVER] = "K_SERVER";
		type_map[K_LOCATION] = "K_LOCATION";
		type_map[K_LISTEN] = "K_LISTEN";
		type_map[K_SERVERNAME] = "K_SERVERNAME";
		type_map[K_ROOT] = "K_ROOT";
		type_map[K_INDEX] = "K_INDEX";
		type_map[K_ERRORPAGE] = "K_ERRORPAGE";
		type_map[K_ALLOWEDMETHODS] = "K_ALLOWEDMETHODS";
		type_map[K_CLIENTMAXSIZEBODY] = "K_CLIENTMAXSIZEBODY";
		type_map[K_CGI] = "K_CGI";
		type_map[K_AUTOINDEX] = "K_AUTOINDEX";
		type_map[K_UPLOADALLOWED] = "K_UPLOADALLOWED";
		type_map[K_RETURN] = "K_RETURN";
		type_map[K_ON] = "K_ON";
		type_map[S_LBRACE] = "S_LBRACE";
		type_map[S_RBRACE] = "S_RBRACE";
		type_map[S_SEMICOLON] = "S_SEMICOLON";
		type_map[V_NUMBER] = "V_NUMBER";
		type_map[V_STR] = "V_STR";
		type_map[V_PATH] = "V_PATH";
		type_map[V_EXTENSION] = "V_EXTENSION";
		type_map[V_STATUSCODE] = "V_STATUSCODE";
		type_map[UNKNOWN] = "UNKNOWN";
	}

	return (type_map[_type]);
}
