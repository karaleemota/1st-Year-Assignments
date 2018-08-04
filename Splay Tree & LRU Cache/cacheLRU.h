#ifndef CACHELRU_H
#define CACHELRU_H

#include "splayTree.h"
#include "bst.h"
 #include <stdexcept>

template <typename Key, typename Value>
class cacheLRU : public SplayTree<Key, Value>
{
	public:
		cacheLRU(int capacity);
		~cacheLRU();
		void put(const std::pair<const Key, Value>& keyValuePair);
		std::pair<const Key, Value> get(const Key& key);
	private:
		Node<Key, Value>* getSmallestLeaf();//retunrs smallest leaf
		Node<Key, Value>* getSmallestLeafHelper(Node<Key, Value>* node);
		int mCapacity;
		int mCurrentSize;//keep track of how many things in cache 
		int mRemoveCount;
};

template <typename Key, typename Value>
cacheLRU<Key, Value>::cacheLRU(int capacity)
{
	mCapacity = capacity;
	mCurrentSize = 0;
}

template <typename Key, typename Value>
cacheLRU<Key, Value>::~cacheLRU()
{

}

template <typename Key, typename Value>
void cacheLRU<Key, Value>::put(const std::pair<const Key, Value>& keyValuePair)
{
	if(mCurrentSize >= mCapacity && 
		BinarySearchTree<Key, Value>::internalFind(keyValuePair.first)==NULL)
	{//key does not exist so we will insert. that means if are at capacity
		//capacity will overfill
		SplayTree<Key, Value>::remove(getSmallestLeaf()->getKey());
		mRemoveCount++;
		mCurrentSize--;
		SplayTree<Key, Value>::insert(keyValuePair);
		mCurrentSize++;
	}
	else if(mCurrentSize < mCapacity &&
	 BinarySearchTree<Key, Value>::internalFind(keyValuePair.first)==NULL)
	{//key does not exist in cache, and capacity will NOT overfill
		SplayTree<Key, Value>::insert(keyValuePair);
		mCurrentSize++;
	}
	else//key already exists
	{//should not actually add it, but will splay
		SplayTree<Key, Value>::insert(keyValuePair);
	}
}

template <typename Key, typename Value>
std::pair<const Key, Value> cacheLRU<Key, Value>::get(const Key& key)
{
	Node<Key, Value>* node = BinarySearchTree<Key, Value>::internalFind(key);
	if(node != NULL)
	{//key exists in tree, return key val pair
		return node->getItem();//return pair of key val of node
	}
	else
	{
		throw std::logic_error("Cannot get, key doesn't exist");
	}
	return node->getItem();
}

template <typename Key, typename Value>
Node<Key, Value>* cacheLRU<Key, Value>::getSmallestLeaf()
{
	if(this->mRoot == NULL)
	{//empty tree
		return NULL;
	}
	return getSmallestLeafHelper(this->mRoot);
}

template <typename Key, typename Value>
Node<Key, Value>* 
cacheLRU<Key, Value>::getSmallestLeafHelper(Node<Key, Value>* node)
{
	if(node == NULL)
	{
		return NULL;
	}
	if(node->getLeft()!=NULL)
	{//node has left child
		return getSmallestLeafHelper(node->getLeft());
	}
	else if(node->getLeft()==NULL && node->getRight()!=NULL)
	{//node only has right node, no left node
		return getSmallestLeafHelper(node->getRight());
	}
	else if(node->getLeft()==NULL && node->getRight()!=NULL)
	{//node has no children
		return node;
	}
	return node;
}

#endif 