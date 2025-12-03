/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/03 17:27:45 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

bool	ErrorPage::findStatus( int target )
{
	if (*_status.cbegin() == target)
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