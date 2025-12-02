/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationTree.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annabrag <annabrag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:27:09 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/02 19:38:43 by annabrag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef struct Node_s
{
	std::string				segment;
	// unsigned int			depth;
	// std::string				full_uri;
	Location*				location;
	std::vector<Node_t*>	children;
	Node_t* 				parent;
}				Node_t;

class LocationTree
{
	private: 
		Node_t*			_root;
		unsigned int	_size;
	
	public : 
		LocationTree( Node_t* root ) : _root(root), _size(1) {}
		~LocationTree() {}

		Node_t*			getRoot() { return (this->_root); }
		unsigned int	getSize() { return (this->_size); }

		Node_t*			createNode( std::string segment, Location* location, Node_t* parent);
		void 			addNode( std::string full_uri, Location* location );
		std::string		rebuildUri( std::vector<std::string> segments );
		void 			deleteNode( Node_t* node );
	
		class	LocationAlreadyExist : public std::exception
		{
			const char	*what() const throw();
		};
};

//creer arbre ? vide ? root ?
//ajouter un noeud
//verifier si le noeud possede le pointeur d'un objet Location
//vider l'arbre
