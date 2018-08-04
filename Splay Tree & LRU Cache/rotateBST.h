#ifndef ROTATEBST_H
#define ROTATEBST_H

#include "bst.h"

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <set>

template <typename Key, typename Value>
class rotateBST : public BinarySearchTree<Key, Value>
{
	public:
		rotateBST();
		bool sameKeys(const rotateBST& t2) const;
		void transform(rotateBST& t2) const;
		
	protected:
		void leftRotate(Node<Key, Value>* r);
		void rightRotate(Node<Key, Value>* r);
	private:
		void sameKeysHelper(Node<Key, Value>* node, std::set<Key> &set) const;
		void transformHelper(rotateBST& t2, Node<Key, Value>* node) const;
		void transformHelper2(rotateBST& t2, Node<Key,Value>* node) const;
};

template <typename Key, typename Value>
rotateBST<Key, Value>::rotateBST()
{
	this->mRoot = NULL;
}

template <typename Key, typename Value>
void rotateBST<Key, Value>::leftRotate(Node<Key, Value>* r)
{
	if(r == NULL)
	{//node to rotate doesnt exist, so do nothing
		return;
	}
	Node<Key, Value>* tempRightChild = r->getRight();
	//Node<Key, Value>* tempLeftChild = r->getLeft();
	if(tempRightChild == NULL)
	{//no right child, so do nothing
		return;
	}
	Node<Key, Value>* tempParent = r->getParent();
	bool rIsRoot = false;
	bool isLeftParent = true;//determine if node is l or r of parent
	if(this->mRoot != r)
	{
		if(tempParent->getLeft() == r)
			isLeftParent = true;
		else if(tempParent->getRight() == r)
			isLeftParent = false;
		if(isLeftParent)
			tempParent->setLeft(r->getRight());
		else
			tempParent->setRight(r->getRight());
		tempRightChild->setParent(tempParent);
	}
	else
	{
		rIsRoot = true;
		tempRightChild->setParent(NULL);
	}
	if(tempRightChild->getLeft() != NULL)
	{
		r->setRight(tempRightChild->getLeft());
		tempRightChild->getLeft()->setParent(r);
	}
	else
	{
		r->setRight(NULL);
	}
	tempRightChild->setLeft(r);
	r->setParent(tempRightChild);
	if(rIsRoot) 
	{
		this->mRoot = tempRightChild;
	}
}

template <typename Key, typename Value>//my rotate right
void rotateBST<Key, Value>::rightRotate(Node<Key, Value>* r)
{
	if(r == NULL)
	{//node to rotate doesnt exist, so do nothing
		return;
	}
	Node<Key, Value>* tempLeftChild = r->getLeft();
	if(tempLeftChild == NULL)
	{//no left child, so do nothing
		return;
	}
	Node<Key, Value>* tempParent = r->getParent();
	bool rIsRoot = false;
	bool isLeftParent = true;//determine if node is l or r of parent
	if(this->mRoot != r)
	{
		if(tempParent->getLeft() == r)
			isLeftParent = true;
		else if(tempParent->getRight() == r)
			isLeftParent = false;
		if(isLeftParent)
			tempParent->setLeft(r->getLeft());
		else
			tempParent->setRight(r->getLeft());
		tempLeftChild->setParent(tempParent);
	}
	else
	{
		rIsRoot = true;
		tempLeftChild->setParent(NULL);
	}
	if(tempLeftChild->getRight() != NULL)
	{
		r->setLeft(tempLeftChild->getRight());
		tempLeftChild->getRight()->setParent(r);
	}
	else
	{
		r->setLeft(NULL);
	}
	tempLeftChild->setRight(r);
	r->setParent(tempLeftChild);
	if(rIsRoot) 
	{
		this->mRoot = tempLeftChild;
	}
}

template <typename Key, typename Value>
bool rotateBST<Key, Value>::sameKeys(const rotateBST& t2) const
{
	std::set<Key> set1;
	std::set<Key> set2;
	sameKeysHelper(this->mRoot,set1);
	sameKeysHelper(t2.mRoot,set2);
	if(set1 == set2)
		return true;
	else
		return false;
}
template <typename Key, typename Value>
void rotateBST<Key, Value>::sameKeysHelper(
	Node<Key, Value>* node, std::set<Key> &set) const
{
	if(node == NULL)
	{//reached end
		return;
	}
	set.insert(node->getKey());
	sameKeysHelper(node->getRight(),set);
	sameKeysHelper(node->getLeft(),set);
}

template <typename Key, typename Value>
void rotateBST<Key, Value>::transform(rotateBST& t2) const
{
	if(!sameKeys(t2))
	{
		return;//t2 and this do not have same keys, so do nothing
	}
	//Perform right rotations on the root node of t2 until it has no left child
	transformHelper(t2, t2.mRoot);//tansforms t2 into linked list

	while(this->mRoot->getKey() != t2.mRoot->getKey())
	{//left roatations on root of t2 till roott2 == rootthis
		t2.leftRotate(t2.mRoot);
	}
	if(t2.mRoot->getLeft() != NULL)
	{
		transformHelper2(t2, t2.mRoot->getLeft());
	}
	if(t2.mRoot->getRight() != NULL)
	{
		transformHelper2(t2, t2.mRoot->getRight());
	}

    //now update all the heights
    t2.mRoot->setHeight(this->mRoot->getHeight());


}
template <typename Key, typename Value>
void rotateBST<Key, Value>::transformHelper(
	rotateBST& t2, Node<Key, Value>* node) const
{
	//rotate node to the right till it doesnt have left children
	Node<Key, Value>* tempnode = node;
	while (tempnode->getLeft() != NULL)
    {//node should be root in first full iteration
        t2.rightRotate(tempnode);//rotate recursively to right
        tempnode = tempnode->getParent();
    }
    if(tempnode->getRight() != NULL)
    {//do this for all right nodes
        transformHelper(t2, tempnode->getRight());
    }

}
template <typename Key, typename Value>
void rotateBST<Key, Value>::transformHelper2(
	rotateBST& t2, Node<Key,Value>* node) const
{//Now perform left rotations on the root node of t2, 
	//until the root of t2 is the same as the root of this
	Node<Key, Value>* tempparent = 
			this->internalFind(node->getParent()->getKey());

    // if this is a mismatch, fix it
    if(node->getParent() != NULL)
    {
        bool isLeftChild = false;
    	if(node->getParent()->getLeft() == node)
    		isLeftChild = true;
    	else if(node->getParent()->getRight() == node)
    		isLeftChild = false;

   		if(isLeftChild && (tempparent->getLeft()->getKey() != node->getKey()))
    	{
        	while (tempparent->getLeft()->getKey() != node->getKey())
        	{
            	t2.rightRotate(node);
            	node = node->getParent();
        	}
    	}
    	else if(!isLeftChild && 
    		(tempparent->getRight()->getKey() != node->getKey()))
    	{
        	while (tempparent->getRight()->getKey() != node->getKey())
        	{
        	    t2.leftRotate(node);
            	node = node->getParent();
        	}
    	}
    }
    // find node in this(thisnode), use find thingy for now
    // node->setheight(thisnode->getheight())
    //update heights
    tempparent = this->internalFind(node->getKey());
    node->setHeight(tempparent->getHeight());
	// now we recurse on left and right children
    if (node->getLeft() != NULL)
    {
        transformHelper2(t2, node->getLeft());       
    }
    if (node->getRight() != NULL)
    {
        transformHelper2(t2, node->getRight());      
    }
}
#endif