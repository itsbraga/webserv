/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/13 23:49:57 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

bool	ErrorPage::findStatus( int target ) const
{
	std::vector<int>::const_iterator it;

	for (it = _status.begin(); it != _status.end(); it++)
	{
		if (*it == target)
			return (true);
	}
	return (false);
}