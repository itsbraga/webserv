/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/05 02:18:00 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

bool	ErrorPage::findStatus( int target )
{
	if (*_status.begin() == target)
		return (true);
	else
	{
		std::vector<int>::iterator it;
		for (it = _status.begin(); it != _status.end(); it++)
		{
			if (*it == target)
				return (true);
		}
		return (false);
	}
}