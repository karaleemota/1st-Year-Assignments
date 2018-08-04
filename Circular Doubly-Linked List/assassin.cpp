#include <iostream>
#include "lliststr.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	ifstream file;
	string line;
	file.open(argv[1]);
	if(!file)
	{
		cout << "Error: file did not open" << endl;
		return 1;
	}

	LListStr* list = new LListStr();
	string command;
	int pos;
	string str;
	while(getline(file,line))
	{
		command = "";
		pos = -100; 
		str = "";

		stringstream stream(line);
		stream >> command;
		if(command == "ADD")
		{//line has ADD pos str command
			stream >> pos >> str;
			list->insert(pos, str);//add player
		}
		else if(command == "REPLACE")
		{//line has REPLACE pos str command
			stream >> pos >> str;
			list->set(pos,str); //replaces player at pos
		}
		else if(command == "ACT")
		{//line has ACT pos command
			stream >> pos;
			if(list->size() <= 1)//only one player
			{
				break; //do nothing
			}
			else if(pos < list->size())// more than one player
			{
				if(pos == list->size()-1)//if last player
				{
					cout<<"Assasinated: "<<list->get(0)<<endl;
					list->remove(0);//remove first player
				}
				else//any other player that's not last
				{
					cout<<"Assasinated: "<<list->get(pos+1)<<endl;
					list->remove(pos+1);
				}
			}
		}
	}
	//game is over
	if(list->size() == 1)
	{
		cout<<"Winner: "<<list->get(0)<<endl;
	}
	else
	{
		cout<<"Remaining Players: "<<list->size()<<endl;
		for(int i = 0; i < list->size(); i++)
		{
			cout<<list->get(i)<<endl;
		}
	}
	file.close();
	delete list; //deallocate
	return 0;
}
