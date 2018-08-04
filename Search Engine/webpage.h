#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <iostream>
#include <set>
#include <vector>
#include <fstream>

class Webpage{
public:
	Webpage();
	Webpage(std::string pagename);
	~Webpage();
	//read from a file into sets,existingpages are webpages that are in index
	void readWebFile(const char* filename,std::set<Webpage*> &existingpages);
	void addoutgoinglink(Webpage* page);//adds page to outgoinglink list
	void addincominglink(Webpage* page);

	std::set<std::string> getwordsSet() const;//get words set
	std::set<Webpage*> getoutgoinglinksSet();//get ougoinglinks set
	std::set<Webpage*> getincominglinksSet();
	std::string getname() const;//returns name of webpage

	bool intersectString(std::set<std::string> words);
	//returns true if page contains 1 of these words
	bool unionString(std::set<std::string> words);
	//returns true if page contaisn all words

	friend std::ostream& operator<< (std::ostream& os, const Webpage& page);
	
private:
	std::set<std::string> words;//all words found in webpage
	std::set<Webpage*> outgoinglinks;//all outgoing links in webpage
	std::set<Webpage*> incominglinks;
	std::string name;//name of this webpage

};
#endif