#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void reverse(std::vector<std::string> lines,int len)
{
	if(len <= 1)
	{
		std::cout<<lines[len-1];
		return;

	}
	else
	{
		std::cout<<lines[len-1]<<" ";
		reverse(lines,len-1);
	}
	return;
}


int main(int argc, char* argv[])
{
	std::ifstream file(argv[1]);
	std::string tempstr = "";
	std::string str = "";
	std::vector<std::string> words;
	int wordamount = 0;
	
		file >> wordamount;
		while(file >> tempstr)
		{
			words.push_back(tempstr);
		}
	
	reverse(words,wordamount);
	file.close();
	return 0;

}
