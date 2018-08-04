#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "bst.h"
#include "rotateBST.h"
#include <iterator>
#include <iostream>

template <typename Key, typename Value>
class SplayTree : public rotateBST<Key, Value>
{
	public:
		SplayTree();//a constructor
		void insert(const std::pair<const Key, Value>& keyValuePair);
		void remove(const Key& key);
		void deleteMinLeaf();
		void deleteMaxLeaf();
		typename SplayTree<Key, Value>::iterator find(const Key& key);
		typename SplayTree<Key, Value>::iterator findMin();
		typename SplayTree<Key, Value>::iterator findMax();

	protected:
		void splay(Node<Key, Value> *r);
	private:
		//Node<Key, Value>* SplayTreeRemove(const Key& key);
		void fixHeight(Node<Key, Value>* parent);
		Node<Key, Value>* getLargestNode();//returns the node with largest key
		Node<Key, Value>* getLargestNodeHelper(Node<Key, Value>* nodePtr) const;
		//same as internal find, but if key is not found return last node we were on
		Node<Key, Value>* find2(const Key& key) const;
		Node<Key, Value>* find2Helper(const Key& key, Node<Key, Value>* node,
		 Node<Key, Value>* parent) const;
};

template <typename Key, typename Value>
SplayTree<Key, Value>::SplayTree()
{

}

template <typename Key, typename Value>
void SplayTree<Key, Value>::insert(const std::pair<const Key, Value>& keyValuePair)
{
	BinarySearchTree<Key, Value>::insert(keyValuePair);
	Node<Key, Value>* insertedNode = 
	BinarySearchTree<Key,Value>::internalFind(keyValuePair.first);
	splay(insertedNode);
}

template <typename Key, typename Value>
void SplayTree<Key, Value>::remove(const Key& key)
{
	Node<Key, Value>* nodeFound = find2(key);
	if(nodeFound == NULL)
	{
		return;
	}
	if(nodeFound->getParent() == NULL)
	{//no splay cuz nodeFound is the root
		BinarySearchTree<Key,Value>::remove(nodeFound->getKey());
		return;
	}
	if(nodeFound->getKey() == key)
	{
		Node<Key, Value>* nodeToSplay = NULL;
		if((nodeFound->getLeft() == NULL && nodeFound->getRight() == NULL)
			|| (nodeFound->getLeft()!=NULL && nodeFound->getRight()==NULL)
			|| (nodeFound->getLeft()==NULL && nodeFound->getRight()!=NULL))
		{//nodeFound has 0 or 1 children 
			nodeToSplay = nodeFound->getParent();
		}
		else if(nodeFound->getLeft()!=NULL && nodeFound->getRight()!=NULL)
		{//nodeFound has 2 children
			Node<Key,Value>* nodeFoundPred = nodeFound->getLeft();
			//find nodefound predecessor here
			while(nodeFoundPred->getRight() != NULL)//finding pred
			{
				nodeFoundPred = nodeFoundPred->getRight();
			}

			if(nodeFound->getLeft()==nodeFoundPred)//direct left child
			{
				nodeToSplay = nodeFoundPred;
			}
			else//not direct left child
			{
				nodeToSplay = nodeFoundPred->getParent();
			}
		}
		BinarySearchTree<Key,Value>::remove(nodeFound->getKey());
		splay(nodeToSplay);
		return;
		//use bst remove, then splay, then return
	}
	else 
	{
		//key not found do nothing
		return;
	}

}



template <typename Key, typename Value>
typename SplayTree<Key, Value>::iterator
 SplayTree<Key,Value>::find(const Key& key)
{
	Node<Key, Value>* node = find2(key);
	splay(node);
	return typename SplayTree<Key, Value>::iterator(node);
}

template <typename Key, typename Value>
typename SplayTree<Key, Value>::iterator SplayTree<Key,Value>::findMin()
{
	Node<Key, Value>* minNode= BinarySearchTree<Key, Value>::getSmallestNode();
	splay(minNode);
	return typename SplayTree<Key, Value>::iterator(minNode);
}

template <typename Key, typename Value>
typename SplayTree<Key, Value>::iterator SplayTree<Key,Value>::findMax()
{
	Node<Key, Value>* maxNode = getLargestNode();
	splay(maxNode);
	return typename SplayTree<Key, Value>::iterator(maxNode);
}

template <typename Key, typename Value>
void SplayTree<Key, Value>::deleteMinLeaf()
{
	Node<Key, Value>* minNode =BinarySearchTree<Key, Value>::getSmallestNode();
	remove(minNode->getKey());
}

template <typename Key, typename Value>
void SplayTree<Key, Value>::deleteMaxLeaf()
{
	Node<Key, Value>* maxNode = getLargestNode();
	remove(maxNode->getKey());
}


template <typename Key, typename Value>
void SplayTree<Key, Value>::splay(Node<Key, Value> *r)
{
	if(r != NULL)
	{
		if(r->getParent() != NULL)//r has parent
		{
			Node<Key, Value>* p = r->getParent();//node's parent
			if(r->getParent()->getParent() != NULL)//r has grandpa
			{
				Node<Key, Value>* g = r->getParent()->getParent();
				if(p->getLeft()==r && g->getLeft()==p)
				{//x is left child of p, and p is left child of g 
					rotateBST<Key, Value>::rightRotate(g);//right rot of gpa
					rotateBST<Key, Value>::rightRotate(p);
					splay(r);
				}
				else if(p->getRight()==r && g->getRight()==p)
				{//x if right child of p, and p is right child of g
					rotateBST<Key, Value>::leftRotate(g);//right rot of gpa
					rotateBST<Key, Value>::leftRotate(p);
					splay(r);
				}
				else if(p->getRight()==r && g->getLeft()==p)
				{//x is right child of p, & p is left child of g 
					rotateBST<Key, Value>::leftRotate(p);
					rotateBST<Key, Value>::rightRotate(g);
					splay(r);
				}
				else if(p->getLeft()==r && g->getRight()==p)
				{//x is left child of p, & p is right child of g
					rotateBST<Key, Value>::rightRotate(p);
					rotateBST<Key, Value>::leftRotate(g);
					splay(r);
				}
			}
			else if(r->getParent()==this->mRoot && this->mRoot->getLeft() == r)
			{//x's parent is the root of the tree
				rotateBST<Key, Value>::rightRotate(p);
			}
			else if(r->getParent() ==this->mRoot && this->mRoot->getRight()==r)
			{//x's parent is the root of the tree
				rotateBST<Key, Value>::leftRotate(p);
			}

		}
	}
}

template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::find2(const Key& key) const
{
	if(this->mRoot == NULL)
		return NULL;
	return find2Helper(key, this->mRoot, NULL);
}

template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::find2Helper(const Key& key,
 Node<Key, Value>* node, Node<Key, Value>* parent) const
{
	if(node != NULL)
	{
		if(node->getKey() == key)
		{
			return node;
		}
		else
		{
			if(key < node->getKey())//current node key is greater than key
			{//so go left to smaller numbers
				return find2Helper(key, node->getLeft(),node);
			}
			else//key > node->getKey
			{
				return find2Helper(key, node->getRight(),node);
			}
		}
	}
	else
	{
		return parent;//pointing to nothing in tree
	}
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::fixHeight(Node<Key, Value>* parent)
{
	Node<Key, Value>* tempnode = parent;
	int tempnodeleftheight = 0;
	int tempnoderightheight = 0;
	while(tempnode != NULL)
	{
		if(tempnode->getLeft() == NULL)
			tempnodeleftheight = 0;
		else
			tempnodeleftheight = tempnode->getLeft()->getHeight();

		if(tempnode->getRight() == NULL)
			tempnoderightheight = 0;
		else
			tempnoderightheight = tempnode->getRight()->getHeight();

		int maxHeight = tempnodeleftheight;
		if(tempnoderightheight > maxHeight)
			maxHeight = tempnoderightheight;

		tempnode->setHeight(maxHeight+1);
		tempnode = tempnode->getParent();
	}
}

template<typename Key, typename Value>//returns the node with largest key
Node<Key, Value>* SplayTree<Key,Value>::getLargestNode()
{
	return getLargestNodeHelper(this->mRoot);
}

template<typename Key, typename Value>
Node<Key, Value>* 
SplayTree<Key,Value>::getLargestNodeHelper(Node<Key, Value>* nodePtr) const
{
	if(this->mRoot == NULL)
	{
		//tree is empty, do nothing
		return this->mRoot;
	}
	else
	{
		if(nodePtr->getRight() != NULL)
		{//traverse down left pointer
			return getLargestNodeHelper(nodePtr->getRight());
		}
		else
		{//largest node found
			return nodePtr;
		}
	}
}

#endif