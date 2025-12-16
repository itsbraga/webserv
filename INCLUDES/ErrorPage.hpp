/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:43:25 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/16 02:33:13 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

# include <string>
# include <vector>

/**************************\
 *	Class
\**************************/

class ErrorPage
{
	private:
		std::vector<int>	_status;
		std::string 		_file;
		
	public: 
		ErrorPage( const std::vector<int>& status, const std::string& file ) : _status( status ), _file( file ) {}
		~ErrorPage() {}
		
		const std::vector<int>&		getStatus() const	{ return (_status); }
		const std::string&			getFile() const		{ return (_file); }

		bool		findStatus( int target ) const;
};