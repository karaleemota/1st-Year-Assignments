#ifndef FUNCTOR_H
#define FUNCTOR_H
#include <string>
#include <iostream>

struct NumStrComp
{
	bool operator()(const std::string& lhs, const std::string& rhs);
};
 
#endif