#include <iostream>
#include "stackstr.h"

using namespace std;

StackStr::StackStr()
{
}

StackStr::~StackStr()
{
	
}

bool StackStr::empty() const
{
	return list_.empty();
}

int StackStr::size() const
{
	return list_.size();
}

void StackStr::push(const std::string &str)
{
	list_.insert(0, str);
	//pos wll always equal size, so constant running time
}

string StackStr::top() const
{
	if(list_.size() == 0)
	{
		return "";
	}
	return list_.get(0);
}

void StackStr::pop()
{
	if(list_.empty())
	{
		return; //do nothing
	}
	list_.remove(0);
	//constant running in remove time bc pos 
	//is always size-1 which is constant
}