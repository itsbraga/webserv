/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationTree.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 23:41:14 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/14 03:24:36 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

LocationTree::LocationTree(Node_t* root)
{
	this->_root = root;
	this->_size = 1;
}

LocationTree::~LocationTree( void )
{
	this->DestroyTree(this->_root);
}

Node_t*	LocationTree::CreateNode(std::string segment, Location *location, Node_t *parent)
{
	Node_t*	new_node = new Node_t(); // a proteger
	new_node->segment = segment;
	new_node->location = location;
	new_node->parent = parent;
	++this->_size;
}

void	LocationTree::AddNode(std::string full_uri, Location *location)
{
	Node_t *current;
	std::vector<std::string> segments;
	std::string segment_to_add;
	std::vector<std::string>::iterator seg_it;
	std::vector<Node_t *>::iterator child_it;
	Node_t *current_match;

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
			current_match = LocationTree::CreateNode(*seg_it, NULL, current);
			current->children.push_back(current_match);
		}
		current = current_match;
	}
	current->children.push_back(LocationTree::CreateNode(segment_to_add, location, current));
}

std::string LocationTree::RebuildUri(std::vector<std::string> segments)
{
	std::string result;
	std::vector<std::string>::iterator it;

	result = "/";
	for (it = segments.begin(); it != segments.end(); ++it)
	{
		result += *it;
		result += "/";
	}
}

Node_t*	LocationTree::GetRoot( void )
{
	return (this->_root);
}

unsigned int	LocationTree::GetSize( void )
{
	return (this->_size);
}

void	LocationTree::DeleteNode(Node_t *node)
{
	if (node->location != NULL)
	{
		delete node->location;
		node->location == NULL;
	}
	delete node;
	if (this->_size > 0)
		--this->_size;
}

void	LocationTree::DestroyTree(Node_t *parent)
{
	if (parent == NULL)
		return ;
	for (int i = 0; i < parent->children.size(); ++i)
		DeleteNode(parent->children[i]);
	
	if (parent->location != NULL)
	{
		delete parent->location;
		parent->location == NULL;
	}
	delete parent;
}