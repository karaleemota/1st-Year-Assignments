#include <iostream>
#include "webpage.h"

using namespace std;

Webpage::Webpage()
{

}

Webpage::~Webpage()
{
	
}

Webpage::Webpage(string pagename)
{
	name = pagename;
	rank = 0;
	//readWebFile(pagename.c_str());
}

void Webpage::readWebFile(const char* filename, set<Webpage*> &existingpages)
{
	ifstream webpageinput;
	webpageinput.open(filename);//argv1 is the webpages
	char tempchar;
	string tempstr = "";
	string strline = "";
	bool parenislink = false;
	if(!webpageinput)
	{
		webpageinput.close();
		return;
	}
	
	while(getline(webpageinput, strline))
	{
		for (unsigned int i=0; i<strline.size(); i++) 
		{
    		tempchar = strline[i];
    		if(tempchar == '(')
			{
				parenislink = true;
				continue;
			
			}
			else if(tempchar == ')')
			{
			//iterate through existingwebpages 2 find page with that name
				set<Webpage*>::iterator it = existingpages.begin();
				for(it= existingpages.begin(); it != existingpages.end(); ++it)
				{
					if((*it)->getname() == tempstr)
					{
						outgoinglinks.insert(*it);
						(*it)->addincominglink(this);
					}
				}
				parenislink = false;
				tempstr = "";
				continue;
			}
			else if(parenislink)
			{
				tempstr += tempchar;
			}
			else if(!parenislink)
			{
				if(isalpha(tempchar) || isdigit(tempchar) )
				{
					tempstr += tolower(tempchar);//convert to lower case
				}
				else if(tempstr != "")//not a-z or 1-9
				{
					words.insert(tempstr);
					tempstr = "";
				}
			}
		}
		if(tempstr != "")//for the last word
		{
			words.insert(tempstr);
			tempstr = "";
		}	
	}
	webpageinput.close();
}

bool Webpage::intersectString(std::set<std::string> wordstosearch)
{
	set<string>::iterator it = wordstosearch.begin();

	for(it = wordstosearch.begin(); it != wordstosearch.end(); ++it)
	{
		if(words.count(*it) == 1)//found wordtosearch in words set
		{
			return true;
		}
	}
	return false;
}

bool Webpage::unionString(std::set<std::string> wordstosearch)
{
	set<string>::iterator it = wordstosearch.begin();
	for(it = wordstosearch.begin(); it != wordstosearch.end(); ++it)
	{
		if(words.count(*it) != 1)//did not find one of the words
		{
			return false;
		}
	}
	return true;
}

std::set<string> Webpage::getwordsSet() const
{
	return words;
}
std::set<Webpage*> Webpage::getoutgoinglinksSet()
{
	return outgoinglinks;
}

std::set<Webpage*> Webpage::getincominglinksSet()
{
	return incominglinks;
}

void Webpage::addoutgoinglink(Webpage* page)//adds page to outgoinglink list
{
	outgoinglinks.insert(page);
}
void Webpage::addincominglink(Webpage* page)
{
	incominglinks.insert(page);
}  

std::string Webpage::getname() const
{
	return name;
}

std::ostream& operator<< (std::ostream& os, const Webpage& page)
{//to PRINT this webpage
	ifstream file(page.getname().c_str());
	string tempstr = "";
	char tempchar;
	bool islink = false;
	if(!file)
	{
		return os;
	}
	while(file.get(tempchar))
	{
		if(tempchar == '(')
		{
			islink = true;
			continue;
		}
		else if(tempchar == ')' && islink)
		{
			islink = false;
			continue;
		}
		if(!islink)
		{
			tempstr += tempchar;
		}		
	}
	os << tempstr;
	file.close();
	return os;
}