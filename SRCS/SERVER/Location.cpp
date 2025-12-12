/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panther <panther@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:17:07 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/11 23:36:26 by panther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

bool	ErrorPage::findStatus( int target ) const
{
	return (std::find( _status.begin(), _status.end(), target ) != _status.end());
}