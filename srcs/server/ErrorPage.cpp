/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:46:39 by art3mis           #+#    #+#             */
/*   Updated: 2025/12/27 18:58:30 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
	----------------------------- [ Setter ] -----------------------------
*/
void	ErrorPage::setFile( std::string file )
{
	if (file.empty())
		return ;

	_file = file;
}

/*
	------------------------- [ Public method ] --------------------------
*/
bool	ErrorPage::findStatus( int target ) const
{
	std::vector<int>::const_iterator it = _status.begin();

	for (; it != _status.end(); it++)
	{
		if (*it == target)
			return (true);
	}
	return (false);
}

/*
	------------------------- [ Handler ] --------------------------
*/

// void	ErrorPageHandler(const Response& response)
// {
	
// }
