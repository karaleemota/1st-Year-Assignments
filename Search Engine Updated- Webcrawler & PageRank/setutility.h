#include <set>
#include <iterator>
template <class T>
bool intersectString(Webpage* webpage, std::set<T> wordstosearch)
{
	class std::set<T>::iterator it;

	for(it = wordstosearch.begin(); it != wordstosearch.end(); ++it)
	{
		if(webpage->getwordsSet().count(*it) == 1)//found wordtosearch in set
		{
			return true;
		}
	}
	return false;
}

template <class T>
bool unionString(Webpage* webpage, std::set<T> wordstosearch)
{
	class std::set<T>::iterator it;
	for(it = wordstosearch.begin(); it != wordstosearch.end(); ++it)
	{
		if(webpage->getwordsSet().count(*it) != 1)//did not find 1 of the wrds
		{
			return false;
		}
	}
	return true;
}

template <class T>
std::set<T> intersectWebPage(std::set<T> set1, std::set<T> set2)
{
	std::set<T> set3;
	class std::set<T>::iterator it;
	for(it = set1.begin(); it != set1.end(); ++it)
	{
		class std::set<T>::iterator it2;
		for(it2 = set2.begin(); it2 != set2.end(); ++it2)
		{
			if((*it) == (*it2))
			{
				set3.insert(*it);
			}
		}
	}
	return set3;
}

template <class T>
std::set<T> unionWebPage(std::set<T> set1, std::set<T> set2)
{
	std::set<T> set3;
	class std::set<T>::iterator it;
	for(it = set1.begin(); it != set1.end(); ++it)
	{
		set3.insert(*it);
	}
	class std::set<T>::iterator it2;
	for(it2 = set2.begin(); it2 != set2.end(); ++it2)
	{
		set3.insert(*it2);
	}
	return set3;
}
