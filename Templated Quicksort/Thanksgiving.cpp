#include <iostream>
#include <vector>
#include <utility>
#include "qsort.h"
#include "LengthMismatch.h"

using namespace std;

std::vector<std::pair<int, int>> assignPlates(std::vector<int> turkeys, 
                                              std::vector<int> potatoes) 
{
     vector<pair<int, int>> vec;
     if(turkeys.size() != potatoes.size())
	 {
		throw LengthMismatch((int)turkeys.size(),(int)potatoes.size());	
	 }
     QuickSort(turkeys, less<int>());
     QuickSort(potatoes,less<int>());
     int calAmount = turkeys[0] + potatoes.back();
     for(long unsigned int i = 0; i < turkeys.size(); i++)
     {
     	if(turkeys[i] + potatoes[potatoes.size()-1-i] == calAmount)
     	{
     		vec.push_back(make_pair(turkeys[i],potatoes[potatoes.size()-1-i]));
     	}
     	else
     	{
     		vec.clear();
     		break;//impossible, so empty vector
     	}
     }
     return vec;  
}
