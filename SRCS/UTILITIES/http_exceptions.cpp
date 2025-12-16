/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_exceptions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 03:09:08 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/16 03:18:56 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"
#include "Response.hpp"
#include "utilities.hpp"

Response*	handleHttpException( const std::exception& e )
{
	// 4xx : Client errors
	if (dynamic_cast<const BadRequestException*>( &e ))
		return (new Response( 400, "Bad Request" ));
	if (dynamic_cast<const ForbiddenException*>( &e ))
		return (new Response( 403, "Forbidden" ));
	if (dynamic_cast<const NotFoundException*>( &e ))
		return (new Response( 404, "Not Found" ));
	if (dynamic_cast<const MethodNotAllowedException*>( &e ))
		return (new Response( 405, "Method Not Allowed" ));
	if (dynamic_cast<const LengthRequiredException*>( &e ))
		return (new Response( 411, "Length Required" ));
	if (dynamic_cast<const PayloadTooLargeException*>( &e ))
		return (new Response( 413, "Payload Too Large" ));
	if (dynamic_cast<const URITooLongException*>( &e ))
		return (new Response( 414, "URI Too Long" ));
	if (dynamic_cast<const TooManyRequestsException*>( &e ))
		return (new Response( 429, "Too Many Requests" ));

	// 5xx : Server errors
	if (dynamic_cast<const InternalServerErrorException*>( &e ))
		return (new Response( 500, "Internal Server Error" ));
	if (dynamic_cast<const NotImplementedException*>( &e ))
		return (new Response( 501, "Not Implemented" ));
	if (dynamic_cast<const BadGatewayException*>( &e ))
		return (new Response( 502, "Bad Gateway" ));
	if (dynamic_cast<const ServiceUnavailableException*>( &e ))
		return (new Response( 503, "Service Unavailable" ));
	if (dynamic_cast<const GatewayTimeoutException*>( &e ))
		return (new Response( 504, "Gateway Timeout" ));
	if (dynamic_cast<const HTTPVersionNotSupportedException*>( &e ))
		return (new Response( 505, "HTTP Version Not Supported" ));

	// Default fallback
	return (new Response( 500, "Internal Server Error" ));
}