/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:43:25 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/29 20:09:56 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Class
\**************************/

class Request;
class Response;
class ServerConfig;

class ErrorPage
{
	private:
		std::vector<int>	_status;
		std::string 		_file;
		
	public: 
		ErrorPage( const std::vector<int>& status, const std::string& file ) : _status( status ), _file( file ) {}
		~ErrorPage() {}

		void		setFile( std::string file );

		std::vector<int>&			getStatus()			{ return (_status); }
		const std::vector<int>&		getStatus() const	{ return (_status); }
		const std::string&			getFile() const		{ return (_file); }

		bool		findStatus( int target ) const;
};

void	ErrorPageHandler( Response& response, const Request& request, const ServerConfig& server );