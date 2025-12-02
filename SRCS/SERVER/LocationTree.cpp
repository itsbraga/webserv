/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationTree.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 23:41:14 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 17:21:10 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Node_t*	LocationTree::createNode( std::string segment, Location* location, Node_t* parent )
{
	Node_t*	new_node = new Node_t(); // a proteger
	new_node->segment = segment;
	new_node->location = location;
	new_node->parent = parent;
	this->_size++;
}

void	LocationTree::addNode( std::string full_uri, Location* location )
{
	Node_t								*current;
	std::vector<std::string>			segments;
	std::string							segment_to_add;
	std::vector<std::string>::iterator	seg_it;
	std::vector<Node_t *>::iterator		child_it;
	Node_t								*current_match;

	current = this->_root;
	segments = split(full_uri, '/');
	segment_to_add = segments.back();
	segments.pop_back();
	for (seg_it = segments.begin(); seg_it != segments.end(); seg_it++)
	{
		current_match = NULL;
		for (child_it = current->children.begin(); child_it != current->children.end(); child_it++)
		{
			if ((*child_it)->segment == *seg_it)
			{
				current_match = *child_it;
				break;
			}
		}
		if (current_match == NULL)
		{
			current_match = LocationTree::createNode(*seg_it, NULL, current);
			current->children.push_back(current_match);
		}
		current = current_match;
	}
	current->children.push_back(LocationTree::createNode(segment_to_add, location, current));
}

std::string		LocationTree::rebuildUri( std::vector<std::string> segments )
{
	std::string result;
	result = "/";

	std::vector<std::string>::iterator it;
	for (it = segments.begin(); it != segments.end(); ++it)
	{
		result += *it;
		result += "/";
	}
}

void	LocationTree::deleteNode( Node_t *node )
{
	if (node->location != NULL)
		delete node->location;
	delete node;
}