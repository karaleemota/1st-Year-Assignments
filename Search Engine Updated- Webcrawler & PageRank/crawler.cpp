#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <set>

using namespace std;

ifstream config;//global file parameters
ifstream index;//seed file
ofstream output;//list of all files reachable from the seeds
set<string> existingLinkNames;//to keep track of what links we have found

void readConfigLine(const char* line);
void readLinkFile(const char* filename);
void readSeed(ifstream &index);//reads seed file and adds links to stack
void DFS(ifstream &link);

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
	if(!index || !output)//error with one of the parameters
	{
		output << "Error- index or output file not open"<<endl;
		return 1;
	}
	readSeed(index);
	index.close();
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
	else if(parameter == "OUTPUT_FILE")
	{
		output.open(value);
	}
}

void readSeed(ifstream &index)
{
	string strline = "";
	while(getline(index,strline))//while loop to look at seed links
	{
		if(existingLinkNames.count(strline) <= 0)//not visited yet
		{
			existingLinkNames.insert(strline);
			ifstream temppage(strline);
			output << strline << endl;
			DFS(temppage);//do dfs on links found outside of seed file
			temppage.close();
		}
		
	}
}

void DFS(ifstream &link)
{//used to dfs search on all pages found,discluding those in seed index
	string strline = "";
	while(getline(link,strline))
	{
		bool islink = false;
		char tempc;
		string linkname = "";
		for(int i = 0; i < (int)strline.size();i++)
		{
			tempc = strline[i];
			if(tempc == '(')
			{
				islink = true;
				continue;
			}
			else if(tempc == ')')
			{
				islink = false;
				if(existingLinkNames.count(linkname) <= 0)//not visited yet
				{
					existingLinkNames.insert(linkname);
					ifstream temppage(linkname);

					if(temppage)
					{
						output << linkname << endl;
						DFS(temppage);
					}
					
					temppage.close();
				}
				linkname = "";
			}
			if(islink)
			{
				linkname += tempc;
			}
		}
			
	}
}



