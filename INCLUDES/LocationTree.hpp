/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationTree.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:27:09 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/14 03:13:09 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once



typedef struct Node_s
{
	std::string 	segment;
	// unsigned int	depth;
	// std::string		full_uri;
	Location		*location;
	std::vector<Node_t*> children;
	Node_t* 		parent;
}	Node_t;


class LocationTree
{
	private: 
		Node_t* _root;
		unsigned int _size;
	
	public : 
		LocationTree(Node_t* root);
		~LocationTree();

		Node_t*					CreateNode(std::string segment, Location *location, Node_t *parent);
		void 					AddNode(std::string full_uri, Location *location);
		std::string				RebuildUri(std::vector<std::string> segments);
		Node_t*					GetRoot();
		unsigned int			GetSize();
		void 					DeleteNode(Node_t *node);
		void					DestroyTree(Node_t *parent);
	
		class	LocationAlreadyExist: public std::exception {};
};

//creer arbre ? vide ? root ?
//ajouter un noeud
//verifier si le noeud possede le pointeur d'un objet Location
//vider l'arbre
