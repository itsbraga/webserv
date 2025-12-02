/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 17:19:34 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool	ErrorPage::findStatus( int target )
{
	if (*this->_status.cbegin() == target)
		return (true);
	else
	{
		std::vector<int>::iterator it;
		for (it = this->_status.begin(); it != this->_status.end(); it++)
		{
			if (*it == target)
				return (true);
		}
		return (false);
	}
}