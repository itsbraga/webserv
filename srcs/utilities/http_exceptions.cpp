/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_exceptions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 03:09:08 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/22 22:01:50 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Response*	handleHttpException( const std::exception& e )
{
	const HttpException* http = dynamic_cast<const HttpException*>( &e );
	if (http)
		return (new Response( http->getCode(), http->getStatus() ));

	return (new Response( 500, "Internal Server Error" ));
}