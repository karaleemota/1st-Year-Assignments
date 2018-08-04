#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
#include <set>
#include <iterator>
#include <vector>
#include "webpage.h"
using namespace std;

void readindex(ifstream &index);
void readqueryline(const char* line, ofstream &output);

set<Webpage*> webpages;//pages read from index will go here

int main(int argc, char* argv[])
{//put in web page class
	ifstream index(argv[1]);
	ofstream output;
	output.open(argv[3]);//the output where query command results are output
	//set<Webpage> webpages;//pages read from index will go here
	string strline = "";
	if(!index)
	{
		output << "Error- index.txt not open"<<endl;
		return 1;
	}
	//QUERY READING FROM INDEX BELOW
	readindex(index);//reads from index, makes webpage objects
	index.close();
	//QUERY
	ifstream query(argv[2]);
	string queryline = "";
	while(getline(query, queryline))
	{
		readqueryline((queryline.c_str()),output);//reads single line of query
	}
	query.close();
	output.close();
	//DEALLOCATE
	set<Webpage*>::iterator it = webpages.begin();
	for(it = webpages.begin(); it != webpages.end(); ++it)
	{
		delete (*it);
	}
}

void readqueryline(const char* line, ofstream &output)
{
	stringstream stream(line);
	string temp = "";
	//first word must be a command, or error
	set<string> words;
	if(!(stream >> temp))//blank line
	{
		output<<"Invalid Query"<<endl;
	}
	else if(temp == "AND")
	{
		if(stream.rdbuf()->in_avail()==0)//"AND" is only word so do single word search
		{
			goto singleWordSearch;
		}
		while(stream >> temp)//now go through all the words and make set
		{
			for(unsigned int i = 0; i < temp.size(); i++)
			{
				temp[i] = tolower(temp[i]);//convert to lowercase
			}
			words.insert(temp);
		}
		set<Webpage*> unionpages;
		set<Webpage*>::iterator it = webpages.begin();
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->unionString(words))//found all words in this page
			{
				unionpages.insert(*it);
			}
		}
		output<< unionpages.size()<<endl;
		set<Webpage*>::iterator it2 = unionpages.begin();
		for(it2 = unionpages.begin(); it2 != unionpages.end(); ++it2)
		{
			output<<(*it2)->getname()<<endl;		
		}
	}
	else if(temp == "OR")
	{
		if(stream.rdbuf()->in_avail()==0)//"OR" is only word so do single word search
		{
			goto singleWordSearch;
		}
		stream<<temp;
		while(stream >> temp)//now go through all the words and make set
		{
			for(unsigned int i = 0; i < temp.size(); i++)
			{
				temp[i] = tolower(temp[i]);//convert to lowercase
			}
			words.insert(temp);
		}
		set<Webpage*> intersectpages;
		set<Webpage*>::iterator it = webpages.begin();
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->intersectString(words))//found all words in this page
			{
				intersectpages.insert(*it);
			}
		}
		output<< intersectpages.size()<<endl;
		set<Webpage*>::iterator it2 = intersectpages.begin();
		for(it2 = intersectpages.begin(); it2 != intersectpages.end(); ++it2)
		{
			output<<(*it2)->getname()<<endl;	
		}
	}
	else if(temp == "PRINT")
	{
		if(stream.rdbuf()->in_avail()==0)//"PRINT" is only word so do single word search
		{
			goto singleWordSearch;
		}
		stream >> temp;//temp will be the webpage's name
		Webpage* temppage;
		set<Webpage*>::iterator it = webpages.begin();
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->getname() == temp)
				temppage = (*it);//found webpage to print
		}
		output<<temppage->getname()<<endl;
		output << *temppage << endl;
	}
	else if(temp == "INCOMING")
	{
		if(stream.rdbuf()->in_avail()==0)//"INCOMING" is only word so do single word search
		{
			goto singleWordSearch;
		}
		stream >> temp;//temp will be the links name
		Webpage* temppage;
		set<Webpage*>::iterator it = webpages.begin();
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->getname() == temp)
				temppage = (*it);
		}
		set<Webpage*> tempset = temppage->getincominglinksSet();
		set<Webpage*>::iterator it2 = tempset.begin();
		output<<tempset.size()<<endl;
		for(it2 = tempset.begin(); it2 != tempset.end(); ++it2)
		{
			output << (*it2)->getname()<<endl;
		}
	}
	else if(temp == "OUTGOING")
	{
		if(stream.rdbuf()->in_avail()==0)//"OUTGOING" is only word so do single word search
		{
			goto singleWordSearch;
		}
		stream >> temp;//tmep is the page whose links we wanna see
		Webpage* temppage;
		set<Webpage*>::iterator it = webpages.begin();
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->getname() == temp)
				temppage = (*it);
		}
		set<Webpage*> tempset = temppage->getoutgoinglinksSet();
		set<Webpage*>::iterator it2 = tempset.begin();
		output<<tempset.size()<<endl;
		for(it2 = tempset.begin(); it2 != tempset.end(); ++it2)
		{
			output << (*it2)->getname()<<endl;
		}
	}
	else
	{
		if(!(stream>>temp))//only one word search
		{
			singleWordSearch:;//jump to this line is its single word search
			for(unsigned int i = 0; i < temp.size(); i++)
			{
				temp[i] = tolower(temp[i]);//convert to lowercase
			}
			set<Webpage*> intersectpages;
			set<Webpage*>::iterator it = webpages.begin();
			for(it = webpages.begin(); it != webpages.end(); ++it)
			{
				if((*it)->getwordsSet().count(temp) == 1)//found word in this page
				{
					intersectpages.insert(*it);
				}
			}
			output<< intersectpages.size()<<endl;
			set<Webpage*>::iterator it2 = intersectpages.begin();
			for(it2=intersectpages.begin(); it2 != intersectpages.end(); ++it2)
			{
				output<<(*it2)->getname()<<endl;	
			}

		}	
		else//WRONG FORMAT ERROR
		{
			output<<"Invalid query"<<endl;
		}
	}
}

void readindex(ifstream &index)//makes webpages for each page in index
{
	string strline = "";
	while(getline(index, strline))
	{
		Webpage* newpage = new Webpage(strline.c_str());
		webpages.insert(newpage);
		
	}
	set<Webpage*>::iterator it = webpages.begin();
	for(it = webpages.begin(); it != webpages.end(); ++it)
	{//parse the webpage.txt and put it into sets in webpage object
		(*it)->readWebFile((*it)->getname().c_str(), webpages);
	}
}