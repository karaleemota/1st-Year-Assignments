#include "functor.h"
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <string>
#include <iostream>

using namespace std;

bool NumStrComp::operator()(const string& lhs, const string& rhs)
	{
		int lhstotal = 0;
		int rhstotal = 0;
		for(unsigned int i = 0; i < lhs.size(); i++)
		{//loop through lhs chars in string
			int tempval = 0;
			char tempc = (char)lhs[i];
			if(isdigit(tempc))
			{
				tempval = (int)tempc - 48;
			}
			else if(isalpha(tempc))//A-Z a-z
			{
				tempc = (char)toupper(tempc);//convert to uppercase
				tempval = ((int)(tempc))-55;
			}
			else//char other than 0-9 a-z
			{
				tempval = 0;
			}
			lhstotal += tempval;
		}
		for(unsigned int j = 0; j < rhs.size(); j++)
		{
			int tempval = 0;
			char tempc = (char)rhs[j];
			if(isdigit(tempc))
			{
				tempval = (int)tempc - 48;
			}
			else if(isalpha(tempc))//A-Z a-z
			{
				tempc = (char)toupper(tempc);//convert to uppercase
				tempval = ((int)(tempc))-55;
			}
			else
			{
				tempval = 0;
			}
			rhstotal += tempval;
		}
		if(rhstotal == lhstotal)//same value compare strings standardly
		{
			return lhs < rhs;
		}
		return lhstotal < rhstotal;
	}
