#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <assert.h>
#include "stackstr.h"

using namespace std;

bool correctParenAndChars(string strline)
{
	char c;
	if(strline == "")
	{
		return false;
	}
	StackStr* stack = new StackStr();
	stringstream line(strline);
	string ctostr = "";
	while(line >> c)
	{
		if(!((c>='a' && c<= 'z') || c=='(' || c==')' || c=='+' ||
		  c=='-' || c=='<' || c=='>' || c== ' '))//chek if char is valid
		{
			while(!stack->empty())
			{
				stack->pop();
		}
			delete stack;
			return false; //wrong type of char
		}
		//check if parantheses match
		if(!islower(c) && c != ' ')
		{
			if(c == '(' )
			{
				ctostr = c;
				stack->push(ctostr);
			}
			else if(c == ')')
			{
				if(stack->empty())
				{
					break;
				} 
				if(c == ')' && stack->top() == "(")
				{
					stack->pop();
				}
				else 
				{
					break;
				}
			}
		}
	}
	if(!stack->empty())
	{
		while(!stack->empty())
		{
			stack->pop();
		}
		delete stack;
		return false;
	}
	else
	{
		while(!stack->empty())
		{
			stack->pop();
		}
		delete stack;
		return true;
	}
}

string concatinate(string str1, string str2)// +
{
	return (str1 + str2);
}

string remove(string str1, string str2)// -
{
	string first = str1;
	string last = str2;
	std::string::size_type i = first.find(last);
	if(i != std::string::npos)
		first.erase(i, last.length());
	return first;
}

string removeEnd(string str)// <
{
	if(str.size() <= 1)
		return str; //do nothing, string is only 1 char long
	else
		return str.substr(0,str.size()-1);
}

string removeStart(string str)// >
{
	if(str.size() <= 1)
		return str;
	else
		return str.substr(1,str.size()-1);//remove front char
}

int main(int argc, char* argv[])
{
	ifstream input;
	ofstream output;
	string strline;
	char c;
	string tempstr = "";
	string top = "";
	string ctostr = "";
	
	input.open(argv[1]);
	output.open(argv[2]);
	if(!input)
	{
		output << "Error: file did not open" << endl;
		input.close();
		output.close();
		return 1;
	}
    while(getline(input, strline))
    {
    	StackStr* stack = new StackStr();
		stringstream line(strline);
		if(line.str() == "" )
		{
			output<<"Malformed"<<endl;
			while(!stack->empty())
			{
				stack->pop();
			}
			delete stack;
			continue;//onto next line of text file
		}
		bool iswhitespace = true;
		for(unsigned int i = 0; i < strline.size();i++)
		{//check if line only has whitespace
			if(!isspace(strline[i]))
				iswhitespace = false;
		}
		if(iswhitespace==true)
		{//whole line is only whitspace, so mal
			output<<"Malformed"<<endl;
			while(!stack->empty())
			{
				stack->pop();
			}
			delete stack;
			continue;
		}
		if(correctParenAndChars(strline))//check if all right chars and parans
		{
			while(line >> c)//each line put strings on stack
			{
				if(c==' ')
				{
					continue;//do nothing if space is found
				}
				else if(islower(c))//check if a-z
				{
					ctostr = c;
					tempstr += ctostr;
					continue;
				}
				else if(!islower(c) && tempstr != "")//not a-z,need to push 
				{
					stack->push(tempstr);//push string onto stack
					tempstr = ""; //reset tempstr for next string
				}
				if(!islower(c))
				{//not a-z, must be () < > + - 
					if(c == '(' || c=='<' || c=='>' || c=='+' || c=='-')
					{
						ctostr = c;
						stack->push(ctostr);
					}
					else if(c == ')')//need to pop until we find (
					{
						if(stack->top()=="("||stack->top()=="+"
							||stack->top()=="-"||stack->top()=="<"
							||stack->top()==">")
						{//before ) there must be a a-z string or malformed
							output<<"Malformed"<<endl;
							goto endOfgetlineLoops;
						}
						string tempopstr1 = "";//temp strings to do operations
						string tempopstr2 = "";//
						string result = "";
						tempopstr1 = stack->top();//asume 1st str must have op
						int numAddOperators = 0;//must be >0 eventually or mal
						int numSubOperators = 0;
						stack->pop();//dont need top anymore cuz we know its a
									//string from the above if statement
						while(stack->top() != "(")
						{
							while(stack->top() == ">"||stack->top()=="<")
							{//remove first element of tempopstr1
								if(stack->top()==">")
								{
								tempopstr1 = removeStart(tempopstr1);
								stack->pop();//pop the >
								}
								else if(stack->top()=="<")
								{
								tempopstr1 = removeEnd(tempopstr1);
								stack->pop();//pop <
								}
								
							}
							if(stack->top() == "-")
							{
								numSubOperators++;
								if(numSubOperators > 1 || numAddOperators>0)
								{//can only be 1 - in pair of paran
									output<<"Malformed"<<endl;
									goto endOfgetlineLoops;
								}//cannot mix + and -
								stack->pop();//pop -
								tempopstr2 = stack->top();
								stack->pop();//pop tempopstr2
								if(tempopstr2=="-"||tempopstr2=="+"||
									tempopstr2=="("||tempopstr2==")"||
									tempopstr2==">"||tempopstr2=="<")
								{//after -, it MUST be another string a-z
									output<<"Malformed"<<endl;//not a-z so mal
									goto endOfgetlineLoops;//onto next line
								}
								else
								{
								   while(stack->top()==">"||stack->top()=="<")
									{
										if(stack->top()==">")
										{
										tempopstr2 = removeStart(tempopstr2);
										stack->pop();//pop the >
										}
										else if(stack->top()=="<")
										{
										tempopstr2 = removeEnd(tempopstr2);
										stack->pop();//pop <
										}
									}
									result=remove(tempopstr2,tempopstr1);
									if(result=="")
									{//cant have empty string as result
										output<<"Malformed"<<endl;
										goto endOfgetlineLoops;
									}
								}
							}
							else if(stack->top() == "+")
							{
								if(numSubOperators==1)
								{//cannot mix + and -
									output<<"Malformed"<<endl;
									goto endOfgetlineLoops;
								}
								numAddOperators++;
								stack->pop();//pop +

								tempopstr2 = stack->top();
								stack->pop();//pop tempopstr2
								if(tempopstr2=="-"||tempopstr2=="+"||
									tempopstr2=="("||tempopstr2==")"||
									tempopstr2==">"||tempopstr2=="<")
								{//after +, it MUST be another string a-z
									output<<"Malformed"<<endl;
									goto endOfgetlineLoops;
								}
								else
								{
								   while(stack->top()==">"||stack->top()=="<")
									{
										if(stack->top()==">")
										{
										tempopstr2 = removeStart(tempopstr2);
										stack->pop();//pop the >
										}
										else if(stack->top()=="<")
										{
										tempopstr2 = removeEnd(tempopstr2);
										stack->pop();//pop <
										}
									}

									if(result=="")
									{
									result=concatinate(tempopstr2,tempopstr1);
									}
								 	else
								 	{
								 	result=concatinate(tempopstr2,result);
								 	}
								 	
								}
							}
							if(!(numAddOperators > 0 || numSubOperators == 1)
								 || numSubOperators > 1)
							{//not right num of + or -
								output<<"Malformed"<<endl;
								goto endOfgetlineLoops;
							}
						}
						if(numSubOperators==0 && numAddOperators==0)
						{//each pair of parans needs an operation
							output<<"Malformed"<<endl;
							goto endOfgetlineLoops;
						}
						stack->pop();//pop paran
						stack->push(result);
					}
				}		
			}
			if(tempstr!="")
			{
				stack->push(tempstr);
				tempstr = "";
			}
			string tempstr2 = "";
			if(stack->size() != 1)//after doing all paran ops
			{
				tempstr2 = stack->top();
				stack->pop();//pop string in stack 
				while(!stack->empty())
				{
					if(stack->top()==">")
					{
						stack->pop();//pop >
						tempstr2 = removeStart(tempstr2);
					}
					else if(stack->top()=="<")
					{
						stack->pop();//pop <
						tempstr2 = removeEnd(tempstr2);
					}
					else
					{//+ and - must be in paran
						output<<"Malformed"<<endl;
						goto endOfgetlineLoops;
					}
				}
				stack->push(tempstr2);//should be oly thing in stack now
			}
		} 
		else
		{
			output<<"Malformed"<<endl;
			goto endOfgetlineLoops;
			//not correct parantheses so mal
		}
		while(!stack->empty())
		{
			output<<stack->top();
			stack->pop();
		}
		output<<endl;
		endOfgetlineLoops:;
		while(!stack->empty())
		{
			stack->pop();
		}
		delete stack;
	}
	input.close();
	output.close();
}