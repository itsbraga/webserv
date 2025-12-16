/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationTree.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: art3mis <art3mis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:27:09 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/16 03:01:33 by art3mis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**************************\
 *	Used libraries
\**************************/

# include <string>
# include <vector>

# include "utilities.hpp"

/**************************\
 *	Struct
\**************************/

class Location;

typedef struct Node_s
{
	std::string				segment;
	// unsigned int			depth;
	// std::string				full_uri;
	Location*				location;
	std::vector<Node_s*>	children;
	Node_s* 				parent;
}				Node_t;

/**************************\
 *	Class
\**************************/

class LocationTree
{
	private: 
		Node_t*			_root;
		unsigned int	_size;
	
	public: 
		LocationTree( Node_t* root ) : _root( root ), _size( 1 ) {}
		~LocationTree() {}

		Node_t*			getRoot() const		{ return (_root); }
		unsigned int	getSize() const		{ return (_size); }

		Node_t*			createNode( const std::string& segment, Location* location, Node_t* parent);
		void 			addNode( const std::string& full_uri, Location* location );
		std::string		rebuildURI( const std::vector<std::string>& segments );
		void 			deleteNode( Node_t* node );
	
		class	LocationAlreadyExist : public std::exception
		{
			const char	*what() const throw();
		};
};