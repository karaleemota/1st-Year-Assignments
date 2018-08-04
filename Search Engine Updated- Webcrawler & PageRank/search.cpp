#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
#include <set>
#include <iterator>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <map>
#include "webpage.h"
#include "setutility.h"
using namespace std;

void readindex(ifstream &index);
void readqueryline(const char* line, ofstream &output);
void readConfigLine(const char* line);
multimap<double,string> pagerank(set<Webpage*> &pages);

set<Webpage*> webpages;//pages read from index will go here

ifstream config;//global file parameters
ifstream index;
ifstream query;
ofstream output;
double E;//restart_probability
double t;//step_number

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		config.open(argv[1]);
	}
	else
	{
		config.open("config.txt");
	}
	if(!config)
	{
		output << "Error- no config file"<<endl;
		return 1;
	}
	//read config.txt file to read parameters
	string configline = "";
	while(getline(config, configline))
	{
		readConfigLine(configline.c_str());
	}
	config.close();
	if(!index || !query || !output)//error with one of the parameters
	{
		output << "Error- index, output or query file not open"<<endl;
		return 1;
	}
	//QUERY READING FROM INDEX BELOW
	readindex(index);//reads from index, makes webpage objects
	index.close();
	//QUERY
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
		if(stream.rdbuf()->in_avail()==0)//"AND" is only word
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
			if(unionString(*it, words))//found all words in this page
			{
				unionpages.insert(*it);
				set<Webpage*> tempOutgoing = (*it)->getoutgoinglinksSet();
				set<Webpage*>::iterator it2;
				for(it2=tempOutgoing.begin();it2!=tempOutgoing.end();++it2)
					unionpages.insert(*it2);
				set<Webpage*> tempIncoming = (*it)->getincominglinksSet();
				for(it2=tempIncoming.begin();it2!=tempIncoming.end();++it2)
					unionpages.insert(*it2);
			}
		}
		output<< unionpages.size()<<endl;
		//to rank the pages after finding which to look up
		multimap<double,string> ranks = pagerank(unionpages);
		std::multimap<double,string>::reverse_iterator rit;
		for(rit = ranks.rbegin();rit!=ranks.rend();++rit)
		{//output ranked page names in order of rankings using reverse iterator
			output<<rit->second<<endl;
		}
	}
	else if(temp == "OR")
	{
		if(stream.rdbuf()->in_avail()==0)//"OR" is only word
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
			if(intersectString(*it, words))//found all words in this page
			{
				intersectpages.insert(*it);
				set<Webpage*> tempOutgoing = (*it)->getoutgoinglinksSet();
				set<Webpage*>::iterator it2;
				for(it2=tempOutgoing.begin();it2!=tempOutgoing.end();++it2)
					intersectpages.insert(*it2);
				set<Webpage*> tempIncoming = (*it)->getincominglinksSet();
				for(it2=tempIncoming.begin();it2!=tempIncoming.end();++it2)
					intersectpages.insert(*it2);
			}
		}
		output<< intersectpages.size()<<endl;
		//to rank the pages after finding which to look up
		multimap<double,string> ranks = pagerank(intersectpages);
		std::multimap<double,string>::reverse_iterator rit;
		for(rit = ranks.rbegin();rit!=ranks.rend();++rit)
		{//output ranked page names in order of rankings using reverse iterator
			output<<rit->second<<endl;
		}
	}
	else if(temp == "PRINT")
	{
		if(stream.rdbuf()->in_avail()==0)//"PRINT" is only word
		{
			goto singleWordSearch;
		}
		stream >> temp;//temp will be the webpage's name
		Webpage* temppage;
		set<Webpage*>::iterator it = webpages.begin();
		bool pageFound = false;
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->getname() == temp)
			{
				temppage = (*it);//found webpage to print
				pageFound = true;
			}
		}
		if(pageFound)
		{
			output<<temppage->getname()<<endl;
			output << *temppage << endl;
		}
		else
		{
			output<<"Invalid page name"<<endl;
		}
		
	}
	else if(temp == "INCOMING")
	{
		if(stream.rdbuf()->in_avail()==0)//"INCOMING" is only word
		{
			goto singleWordSearch;
		}
		stream >> temp;//temp will be the links name
		Webpage* temppage;
		set<Webpage*>::iterator it = webpages.begin();
		bool pageFound = false;
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->getname() == temp)
			{
				temppage = (*it);
				pageFound = true;
			}
		}
		if(pageFound)
		{
			set<Webpage*> tempset = temppage->getincominglinksSet();
			set<Webpage*>::iterator it2 = tempset.begin();
			output<<tempset.size()<<endl;
			for(it2 = tempset.begin(); it2 != tempset.end(); ++it2)
			{
				output << (*it2)->getname()<<endl;
			}
		}
		else
		{
			output << "Invalid page name"<<endl;
		}
	}
	else if(temp == "OUTGOING")
	{
		if(stream.rdbuf()->in_avail()==0)//"OUTGOING" is only word
		{
			goto singleWordSearch;
		}
		stream >> temp;//tmep is the page whose links we wanna see
		Webpage* temppage;
		set<Webpage*>::iterator it = webpages.begin();
		bool pageFound = false;
		for(it = webpages.begin(); it != webpages.end(); ++it)
		{
			if((*it)->getname() == temp)
			{
				temppage = (*it);//found webpage with the name from query
				pageFound = true;
			}
		}
		if(pageFound)
		{
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
			output << "Invalid page name" << endl;
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
				if((*it)->getwordsSet().count(temp) >= 1)//found word in pg
				{			
					intersectpages.insert(*it);
					set<Webpage*> tempOutgoing = (*it)->getoutgoinglinksSet();
					set<Webpage*>::iterator it2;
					for(it2=tempOutgoing.begin();it2!=tempOutgoing.end();++it2)
						intersectpages.insert(*it2);
					set<Webpage*> tempIncoming = (*it)->getincominglinksSet();
					for(it2=tempIncoming.begin();it2!=tempIncoming.end();++it2)
						intersectpages.insert(*it2);
				}
			}
			output<< intersectpages.size()<<endl;
			multimap<double,string> ranks = pagerank(intersectpages);
			std::multimap<double,string>::reverse_iterator rit;
			for(rit = ranks.rbegin();rit!=ranks.rend();++rit)
			{//output ranked page names in order of rankings using reverse it
				output<<rit->second<<endl;
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

//reads 1 line of config file for parameters, initialize the parametrs
void readConfigLine(const char* line)
{
	string strline = line;
	string parameter = "";
	string value = "";
	bool equalFound = false;
	for(unsigned int i = 0; i < strline.size(); i++)//iterate through chars
	{
		if(strline[i] == '#')//just a comment so rest of line not important
			break;
		if(strline[i] == ' ')//space, can skip it
			continue;
		if(strline[i] == '=')
		{
			equalFound = true;
			continue;
		}
		if(!equalFound)//must be paramter, = not found yet
		{
			parameter += strline[i];
		}
		else if(equalFound)//everything after = must be value
		{
			value += strline[i];
		}
	}
	if(parameter == "INDEX_FILE")
	{
		index.open(value);
	}
	else if(parameter == "QUERY_FILE")
	{
		query.open(value);
	}
	else if(parameter == "OUTPUT_FILE")
	{
		output.open(value);
	}
	else if(parameter == "RESTART_PROBABILITY")
	{
		E = atof(value.c_str());
	}
	else if(parameter == "STEP_NUMBER")
	{
		t = atof(value.c_str());
	}
}

multimap<double, string> pagerank(set<Webpage*> &pages)
{
	set<Webpage*>::iterator it = pages.begin();
	double n = (double)pages.size();//# of 'nodes' in graph
	map<string,double> rankmap;//used to map each ranking to the name of page
	string tempname;
	for(it = pages.begin(); it != pages.end(); ++it)
	{//initialize each pagerank to 1/n
		rankmap.insert(std::pair<string,double>((*it)->getname(),1.0/n));
	}
	map<string,double> rankmap2 = rankmap;//temp map updated for each iter
	for(int i = 0; i < (int)t; i++)//one iteration is one step
	{
		set<Webpage*>::iterator it;
		for(it = pages.begin();it!= pages.end();++it)
		{
			//below will be the sigma part of the equation
			double neighborsranksum = 0;
			tempname = (*it)->getname();
			set<Webpage*> neighbors = 
			   intersectWebPage((*it)->getincominglinksSet(),pages);
			neighbors.insert(*it);//self loop thing

			set<Webpage*>::iterator it2 = neighbors.begin();
			for(it2 = neighbors.begin(); it2 != neighbors.end();++it2)
			{
				set<Webpage*> outgoingit2 =
				   intersectWebPage((*it2)->getoutgoinglinksSet(),pages);
				outgoingit2.insert(*it2);//self loop thing

				neighborsranksum += rankmap[(*it2)->getname()]*
				(1/((double)(outgoingit2.size())));
			}
			rankmap2[tempname]=(1.0-E)*(neighborsranksum)+(E*(1.0/n));
		}
		rankmap = rankmap2;//update our map using temp map
	}
	//put map values into two vector so we can sort the ranks
	multimap<double,string> mymultimap;
	map<string,double>::iterator pos;
    for (pos = rankmap.begin(); pos != rankmap.end(); ++pos) 
    {
    	mymultimap.insert(std::pair<double,string>(pos->second,pos->first));
   	}
   	return mymultimap;//multimap returned will sort the page ranks automaticaly
}