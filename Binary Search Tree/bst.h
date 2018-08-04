#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <cmath>
#include <iostream>

/**
* A templated class for a Node in a search tree. 
The getters for parent/left/right are virtual so that they
* can be overridden for future kinds of search trees, 
such as Red Black trees, Splay trees, and AVL trees.
*/
template <typename Key, typename Value>
class Node 
{
public:
	Node(const Key& key, const Value& value, Node<Key, Value>* parent);
	virtual ~Node();

	const std::pair<const Key, Value>& getItem() const;
	std::pair<const Key, Value>& getItem();
	const Key& getKey() const;
	const Value& getValue() const;
	Value& getValue();

	virtual Node<Key, Value>* getParent() const;
	virtual Node<Key, Value>* getLeft() const;
	virtual Node<Key, Value>* getRight() const;
	int getHeight() const;

	void setParent(Node<Key, Value>* parent);
	void setLeft(Node<Key, Value>* left);
	void setRight(Node<Key, Value>* right);
	void setValue(const Value &value);
	void setHeight(int height);

protected:
	std::pair<const Key, Value> mItem;
	Node<Key, Value>* mParent;
	Node<Key, Value>* mLeft;
	Node<Key, Value>* mRight;
	int mHeight;
};

/*
	-----------------------------------------
	Begin implementations for the Node class.
	-----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, 
	const Value& value, Node<Key, Value>* parent) 
	: mItem(key, value)
	, mParent(parent)
	, mLeft(NULL)
	, mRight(NULL)
	, mHeight(1)
{ 

} 

/**
* Destructor, which does not need to do anything since the pointers 
inside of a node
* are only used as references to existing nodes. 
The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node() 
{ 

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const 
{ 
	return mItem; 
}

/**
* A non-const getter for the item. 
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem() 
{ 
	return mItem; 
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const 
{ 
	return mItem.first; 
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const 
{ 
	return mItem.second; 
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
	return mItem.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const 
{ 
	return mParent; 
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const 
{ 
	return mLeft; 
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const 
{ 
	return mRight; 
}

/**
* A const getter for the height.
*/
template<typename Key, typename Value>
int Node<Key, Value>::getHeight() const
{
	return mHeight;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent) 
{
	mParent = parent; 
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left) 
{ 
	mLeft = left; 
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right) 
{ 
	mRight = right; 
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value) 
{ 
	mItem.second = value; 
}

/**
* A setter for the height of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setHeight(int height) 
{ 
	mHeight = height;
}

/*
	---------------------------------------
	End implementations for the Node class.
	---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree 
{
	public:
		BinarySearchTree(); //TODO
		virtual ~BinarySearchTree(); //TODO
		int height(); //TODO 
		bool isBalanced(); //TODO 
		virtual void insert(const std::pair<const Key, Value>& keyValuePair);
		virtual void remove(const Key& key); //TODO
		void clear(); //TODO
		void print() const;

	public:
		/**
		* An internal iterator class for traversing the contents of the BST
		*/
		class iterator 
		{
			public:
				iterator(Node<Key,Value>* ptr);
				iterator();

				std::pair<const Key, Value>& operator*();
				std::pair<const Key, Value>* operator->();

				bool operator==(const iterator& rhs) const;
				bool operator!=(const iterator& rhs) const;
				iterator& operator=(const iterator& rhs);

				iterator& operator++();

			protected:
				Node<Key, Value>* mCurrent;
		};

	public:
		iterator begin();
		iterator end();
		iterator find(const Key& key) const;

	protected:
		Node<Key, Value>* internalFind(const Key& key) const; //TODO
		Node<Key, Value>* getSmallestNode() const; //TODO
		void printRoot (Node<Key, Value>* root) const;

	protected:
		Node<Key, Value>* mRoot;
	private:
		bool isBalancedhelper(Node<Key, Value>* node);//helper to isBalanced()
		void fixHeight(Node<Key, Value>* parent);
		Node<Key, Value>* getSmallestNodeHelper(
			Node<Key, Value>* nodePtr) const;
		Node<Key, Value>* internalFindHelper(
			const Key& key, Node<Key, Value>* node) const;
		void clearHelper(Node<Key, Value>* node);
};

/*
	---------------------------------------------------------------
	Begin implementations for the BinarySearchTree::iterator class.
	---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value>* ptr)
	: mCurrent(ptr)
{ 

}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator()
	: mCurrent(NULL)
{ 

}

/**
* Provides access to the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& BinarySearchTree<
				Key, Value>::iterator::operator*() 
{ 
	return mCurrent->getItem();
}

/**
* Provides access to the address of the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>* BinarySearchTree<Key,
					 Value>::iterator::operator->() 
{ 
	return &(mCurrent->getItem()); 
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(
	const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent == rhs.mCurrent;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(
	const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent != rhs.mCurrent;
}

/**
* Sets one iterator equal to another iterator.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator &BinarySearchTree<Key,
 Value>::iterator::operator=(const BinarySearchTree<Key, Value>::iterator& rhs)
{
	this->mCurrent = rhs.mCurrent;
	return *this;
}

/**
* Advances the iterator's location using an in-order traversal.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<
									Key, Value>::iterator::operator++()
{
	if(mCurrent->getRight() != NULL)
	{
		mCurrent = mCurrent->getRight();
		while(mCurrent->getLeft() != NULL)
		{
			mCurrent = mCurrent->getLeft();
		}
	}
	else if(mCurrent->getRight() == NULL)
	{
		Node<Key, Value>* parent = mCurrent->getParent();
		while(parent != NULL && mCurrent == parent->getRight())
		{
			mCurrent = parent;
			parent = parent->getParent();
		}
		mCurrent = parent;
	}
	return *this;
}

/*
	-------------------------------------------------------------
	End implementations for the BinarySearchTree::iterator class.
	-------------------------------------------------------------
*/

/*
	-----------------------------------------------------
	Begin implementations for the BinarySearchTree class.
	-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{ 
	mRoot = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() 
{ 
	clear();
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{ 
	printRoot(mRoot);
	std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<
												Key, Value>::begin()
{
	BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
	return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<
													Key, Value>::end()
{
	BinarySearchTree<Key, Value>::iterator end(NULL);
	return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key
								, Value>::find(const Key& key) const 
{
	Node<Key, Value>* curr = internalFind(key);
	BinarySearchTree<Key, Value>::iterator it(curr);
	return it;
}

/**
* An method to return the height of the BST.
*/
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::height()
{
	if(mRoot == NULL)
		return 0;
	else
		return mRoot->getHeight();
}


/**
* An method to checks if the BST is balanced. 
This method returns true if and only if the BST is balanced.
*/
// This function will return true if an only if the tree is balanced. 
// Recall from lecture that a tree is balanced 
// if and only if for all subtrees
 // with root v |height(left_subtree_v) - height(right_subtree_v)| <= 1
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced()//O(n) TIME!
{
	// TODO
	if(mRoot == NULL)//empty, so balanced
	{
		return true;
	}
	else
	{
		return isBalancedhelper(mRoot);
	}
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalancedhelper(
									Node<Key, Value>* node)
{
	Node<Key, Value>* templeft = NULL;
	Node<Key, Value>* tempright = NULL;
	if(node != NULL)
	{
		templeft = node->getLeft();
		tempright = node->getRight();
	}
	int templeftheight = 0;
	int temprightheight = 0;

	if(templeft == NULL)
		templeftheight = 0;
	else
		templeftheight = templeft->getHeight();
	if(tempright == NULL)
		temprightheight = 0;
	else
		temprightheight = tempright->getHeight();
	if(!(abs(templeftheight - temprightheight) <= 1))
	{
		return false;
	}
	else
	{
		if(templeft != NULL)
		{
			return isBalancedhelper(templeft);
		}
		if(tempright != NULL)
		{
			return isBalancedhelper(tempright);
		}	
	}
	return true;
}
/**
* An insert method to insert into a Binary Search Tree. 
The tree will not remain balanced when 
* inserting.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(
	const std::pair<const Key, Value>& keyValuePair)
{
	// TODO
	bool inserted = false;
	if(mRoot == NULL)//empty tree
	{
		mRoot = new Node<Key,Value>(
			keyValuePair.first, keyValuePair.second, NULL);
		mRoot->setLeft(NULL);
		mRoot->setRight(NULL);
		mRoot->setHeight(1);
		inserted = true;
		return;
	}
	else//not empty tree
	{
		Node<Key, Value>* tempnode = mRoot;
		while(!inserted)
		{
			if(keyValuePair.first == tempnode->getKey())
			{
				inserted = true;//do nothing, key exists
				break;
			}
			else if(keyValuePair.first < tempnode->getKey())
			{
				if(tempnode->getLeft() == NULL)
				{//insert here
					Node<Key, Value>* newnode = new Node<Key,Value>(
						keyValuePair.first,keyValuePair.second,tempnode);
					newnode->setParent(tempnode);
					newnode->setHeight(1);
					tempnode->setLeft(newnode);
					inserted = true;
					break;
				}
				else
				{
					tempnode = tempnode->getLeft();
				}
			}
			else if(keyValuePair.first > tempnode->getKey())
			{
				if(tempnode->getRight() == NULL)
				{
					Node<Key, Value>* newnode = new Node<Key,Value>(
							keyValuePair.first,keyValuePair.second,tempnode);
					newnode->setParent(tempnode);
					newnode->setHeight(1);
					tempnode->setRight(newnode);
					inserted = true;
				}
				else
				{
					tempnode = tempnode->getRight();
				}
			}
		}
		//UPDATE ALL THE HEIGHTS
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
	
}

/**
* An remove method to remove a specific key from a Binary Search Tree. 
The tree may not remain balanced after
* removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
	//tree is empty
	if(mRoot == NULL)
	{
		return;
	}
	//only one node
	else if(height() == 1)
	{
		delete mRoot;
		mRoot = NULL;
		return;
	}
	Node<Key, Value>* tempnode = mRoot;
	while(1)
	{
		if(tempnode == NULL)
		{//node with key value does not exist
			return;
		}
		else if(key < tempnode->getKey())
		{
			tempnode = tempnode->getLeft();//continue searching for key
		}
		else if(key > tempnode->getKey())
		{
			tempnode = tempnode->getRight();
		}
		else if(tempnode->getKey() == key)//found the key to remove's node
		{
			Node<Key, Value>* parent = tempnode->getParent();
			if(parent == NULL)//node to delete is the root
			{
				if(tempnode->getLeft()!=NULL && tempnode->getRight()!=NULL)
				{//has two children
					Node<Key, Value>* pred = tempnode->getLeft();
					while(pred->getRight() != NULL)
					{
						pred = pred->getRight();
					}
					Node<Key, Value>* predParent = pred->getParent();
					if(predParent == tempnode)//pred is connected to node 
					{//so just promote
						pred->setParent(NULL);
						pred->setRight(tempnode->getRight());
						tempnode->getRight()->setParent(pred);
						mRoot = pred;
						//FIX HEIGHT HERE!!!!!!!!!!!
						fixHeight(pred);
						delete tempnode;
						return;
					}
					if(pred->getLeft() != NULL)
					{
						predParent->setRight(pred->getLeft());
						pred->getLeft()->setParent(predParent);
					}
					else
					{
						predParent->setRight(NULL);
					}
					pred->setRight(tempnode->getRight());
					pred->setLeft(tempnode->getLeft());
					tempnode->getRight()->setParent(pred);
					tempnode->getLeft()->setParent(pred);
					pred->setParent(NULL);

					mRoot = pred;

					//FIX HEIGHTS HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					fixHeight(predParent);

				}
			   else if(tempnode->getLeft()!=NULL && tempnode->getRight()==NULL)
                { // nod only has a left child
                    tempnode->getLeft()->setParent(NULL);
                    mRoot = tempnode->getLeft();
                }
               else if(tempnode->getLeft()==NULL && tempnode->getRight()!=NULL)
                { // has only a right child
                    tempnode->getRight()->setParent(NULL);
                    mRoot = tempnode->getRight();
                }
                delete tempnode;
                return;
			}
			//node is a leaf node
			if(tempnode->getLeft() == NULL && tempnode->getRight() == NULL)
			{
				if(parent->getLeft() == tempnode)
				{//left child is node to delete
					parent->setLeft(NULL);
				}
				else
				{
					parent->setRight(NULL);
				}

				//FIX HEIGT HERE !!!!!!!!!!!!!!!
				fixHeight(parent);
			}
			//has exactly one child
			else if(tempnode->getLeft() != NULL && tempnode->getRight()==NULL)
			{//has left child but no right
				if(parent->getLeft() == tempnode)
				{
					parent->setLeft(tempnode->getLeft());
				}
				else
				{
					parent->setRight(tempnode->getLeft());
				}
				tempnode->getLeft()->setParent(parent);

				//FIX HEIGHTS HERE 	~!!!!!!!!!!!!!!!!!
				fixHeight(parent);
			}
			else if(tempnode->getLeft()== NULL && tempnode->getRight() != NULL)
			{//has right child, no left child
				if(parent->getLeft() == tempnode)
				{
					parent->setLeft(tempnode->getRight());
				}
				else
				{
					parent->setRight(tempnode->getRight());
				}

				tempnode->getRight()->setParent(parent);

				//FIX HEIGHTS HERE !!!!!!!!!!!!!!!
				fixHeight(parent);
			}
			else//node has two children and is not root
			{
				Node<Key, Value>* pred = tempnode->getLeft();
				while(pred->getRight() != NULL)
				{
					pred = pred->getRight();
				}
				Node<Key, Value>* predParent = pred->getParent();
				if(predParent == tempnode)//pred is directly connected to node
				{
					pred->setParent(parent);
                    if (parent->getLeft() == tempnode)
                    {
                        parent->setLeft(pred);
                    }
                    else
                    {
                        parent->setRight(pred);
                    }
                    
                    pred->setRight(tempnode->getRight());
                    tempnode->getRight()->setParent(pred);

                    //FIX HEIGHTS HERE 
                    fixHeight(parent);
                    delete tempnode;
                    return;
				}
				//pred has left children, so promote it
				if(pred->getLeft() !=NULL)
				{
					predParent->setRight(pred->getLeft());
					pred->getLeft()->setParent(predParent);
				}
				else
				{
					predParent->setRight(NULL);
				}
				//'swap' pred with node to be delete
				pred->setParent(parent);
				if(parent->getLeft() == tempnode)//node is left child
				{
					parent->setLeft(pred);
				}
				else//node is right child
				{
					parent->setRight(pred);
				}
				pred->setLeft(tempnode->getLeft());
				tempnode->getLeft()->setParent(pred);
				pred->setRight(tempnode->getRight());
				tempnode->getRight()->setParent(pred);

				//FIX HEIGHTS HERE 
				fixHeight(predParent);
			}
			delete tempnode;
			return;

		}
		else
		{
			return;//key was not found in tree
		}
	}
}
/**
* A method to remove all contents of the tree and reset the values in the tree
* for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
	clearHelper(mRoot);
	mRoot = NULL;
}
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value>* node)
{
	if(node == NULL)
	{
		return;//empty node, do nothing
	}
	else//node exists
	{
		Node<Key, Value>* temprightchild = node->getRight();
		Node<Key, Value>* templeftchild = node->getLeft();
		delete(node);
		clearHelper(temprightchild);
		clearHelper(templeftchild);
	}
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::fixHeight(Node<Key, Value>* parent)
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

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
	return getSmallestNodeHelper(mRoot);
}
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNodeHelper(
											 Node<Key, Value>* nodePtr) const
{
	if(mRoot == NULL)
	{
		//tree is empty, do nothing
		return mRoot;
	}
	else
	{
		if(nodePtr->getLeft() != NULL)
		{//traverse down left pointer
			return getSmallestNodeHelper(nodePtr->getLeft());
		}
		else
		{//smallest node found
			return nodePtr;
		}
	}
}

/**
* Helper function to find a node with given key, k and 
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(
												const Key& key) const 
{
	return internalFindHelper(key, mRoot);
}
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFindHelper(
						const Key& key, Node<Key, Value>* node) const
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
				return internalFindHelper(key, node->getLeft());
			}
			else//key > node->getKey
			{
				return internalFindHelper(key, node->getRight());
			}
		}
	}
	else
	{
		return NULL;//pointing to nothing in tree
	}
}


/**
* Helper function to print the tree's contents
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::printRoot(Node<Key, Value>* root) const
{
	if (root != NULL)
	{
		std::cout << "[";
		printRoot (root->getLeft());
		std::cout<< " (" << root->getKey() << ", " << root->getValue() << ") ";
		printRoot (root->getRight());
		std::cout << "]";
	}
}

/*
	---------------------------------------------------
	End implementations for the BinarySearchTree class.
	---------------------------------------------------
*/

#endif
