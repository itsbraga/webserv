/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/08/19 22:06:57 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "webserv.hpp"

std::vector<int>	ErrorPage::getStatus()
{
	return (this->_status);
}

std::string	ErrorPage::getFile()
{
	return(this->_file);
}

bool ErrorPage::findStatus(int target)
{
	if (*this->_status.cbegin() == target)
		return (true);
	else
	{
		std::vector<int>::iterator it;
		for(it = this->_status.begin(); it != this->_status.end(); it++)
		{
			if (*it == target)
				return (true);
		}
		return(false);
	}
}