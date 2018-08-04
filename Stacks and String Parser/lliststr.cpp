#include <iostream>
#include "lliststr.h"
#include <string>

using namespace std;


LListStr::LListStr()
{
	size_ = 0;
	head_ = NULL;
	tail_ = NULL;
}

LListStr::~LListStr()
{
	Item *ptr = head_;
 	Item *temp;
	while(size_ != 0)
	{
 		temp = ptr;
 		ptr = ptr->next;
 		delete temp; // release the memory pointed to by temp
 		size_--;
 	} 
}

int LListStr::size() const
{
	return size_;
}

bool LListStr::empty() const
{
	return size_ == 0;
}

void LListStr::insert(int pos, const string &val)
{
	if(pos < 0 || pos > size_)
	{
		return;
	}
	Item* newItem = new Item();
	newItem->val = val;
	newItem->prev = NULL;
	newItem->next = NULL;

	if(size_ == 0)
	{//if list is empty
		newItem->prev = NULL;
		newItem->next = NULL;
		head_ = newItem;
		tail_ = newItem; 
		size_++;
		return;
	}
	if(pos == 0)
	{//insert in beginning of the list
		newItem->prev = tail_;
		newItem->next = head_;
		head_->prev = newItem;
		head_ = newItem;
		tail_->next = newItem;
		size_++;
	}
	else if(pos == size_)
	{//last element
		tail_->next = newItem;
		newItem->prev = tail_;
		newItem->next = head_;
		tail_ = newItem;
		head_->prev = newItem;
		size_++;
	}
	else
	{
		Item* temp1 = head_;
		Item* temp2 = NULL;
		for(int i = 0; i < pos-1; i++)
		{
			temp1 = temp1->next;
		}
		temp2 = temp1->next;
		temp1->next = newItem;
		newItem->prev = temp1;
		newItem->next = temp2;
		temp2->prev = newItem;
		size_++;
		return;
	}
}

void LListStr::remove(int pos)
{
	if(pos < 0 || pos > size_-1)//pos out of bounds
	{
		return;//do nothing
	}
	if(head_ == NULL && tail_ == NULL)
	{
		delete head_;
		delete tail_;
		return;//list is empty, cannot delete anything
	}
	if(size_ == 1)
	{//only one element
		delete head_;
		size_--;
	}
	else if(pos == 0)
	{//remove first element in list larger than 1
		Item* temp = head_;
		tail_->next = temp->next;
		temp->next->prev = tail_;
		head_ = temp->next;
		delete temp;
		size_--;
	}
	else if(pos == size_-1)
	{//last element of list greater than 1
		Item* temp = tail_;
		tail_->prev->next = head_;
		head_->prev = tail_->prev;
		tail_ = temp->prev;
		delete temp;
		size_--;
	}
	else//list not empty, not first or last element
	{
		Item* temp = head_;
		for(int i = 0; i < pos; i++)
		{
			temp = temp->next;
		}
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		size_--;
		delete temp;
		return;
		
	}
}

void LListStr::set(int pos, const string &val)
{
	if(pos < 0 || pos > size_-1)
	{
		return; //out of range do nothing
	}
	Item* temp = head_;
	for(int i = 0; i < pos; i++)
	{
		temp = temp->next;
	}
	if(size_ == 1)//only one element in list
	{
		head_-> val = val;
		tail_->val = val;
	}
	else
	{
		if(temp == head_)
		{
			head_->val = val;
		}
		else if(temp == tail_)
		{
			tail_->val = val;
		}
		else//not first or last element
		{
			temp->next->prev->val = val;
		}
	}
}

string LListStr::get(int pos) const
{
	if(pos < 0 || pos > size_)
	{
		return ""; //out of range do nothing
	}
	Item* temp = head_;
	for(int i = 0; i < pos; i++)
	{ 
		temp = temp->next;
	}
	string value = temp->val;
	return value;
}