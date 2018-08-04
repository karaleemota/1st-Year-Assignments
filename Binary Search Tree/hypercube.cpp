#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <queue>
#include <set>

using namespace std;

struct Node
{
public:
	int f;
	int g;
	int h;
	Node* predecessor;
	string value;
};
struct Comparator
{
	bool operator()(const Node* lhs,const Node* rhs)
	{
		if((lhs->f == rhs->f) && (lhs->h == rhs->h))
		{
			for(unsigned int i = 0; i < rhs->value.size(); i++)
			{
				if(rhs->value[i] != lhs->value[i])
				{
					if(rhs->value[i] == '1')
					{
						return false;
					}
					else
					{
						return true;
					}
				}
			}
		}
		if(lhs->f == rhs->f)
		{
			return lhs->h > rhs->h;
		}
		else
		{
			return lhs->f > rhs->f;
		}
	}
};

int findH(const Node* node);
set<string> getValidNeighbors(Node* node,const vector<string> &permittedNodes);

int main(int argc, char* argv[])
{
	string start = argv[1];
	string finish = "";
	for(unsigned int i = 0; i < start.size(); i++)
	{
		finish += '1';
	}
	ifstream permittedFile(argv[2]);
	if(!permittedFile)
	{
		return -1;//file did not open properly, so exit program
	}
	vector<string> permittedNodes;
	string tempPermittedNode = "";

	getline(permittedFile,tempPermittedNode);//get # of permitted nodes
	while(getline(permittedFile,tempPermittedNode))//read thru permitted nodes
	{
		permittedNodes.push_back(tempPermittedNode);
	}//now vector should contain all permitted nodes

	priority_queue<Node*,vector<Node*>,Comparator> priorityqueue;
	//add start to priority queue
	Node* startnode = new Node();//make node for start string thing
	startnode->value = start;
	startnode->g = 0;
	startnode->h = findH(startnode);
	startnode->f = startnode->g + startnode->h;
	startnode->predecessor = NULL;//start, so no predecessor
	priorityqueue.push(startnode);//add start to priorityqueue
	//make vector to keep track of all nodes so we can delete them later
	vector<Node*> allDynamicallyAllocatedNodes;
	allDynamicallyAllocatedNodes.push_back(startnode);

	int expansionNum = 0;
	set<string> visited;//keep track of what nodes have been visited
	bool foundFinish = false;//keeptrack of if we've found finish in PQ
	Node* backtraceTempNode;//used later to backtrace if path has been found
	while(!priorityqueue.empty())
	{
		Node* top = priorityqueue.top();
		priorityqueue.pop();
		if(top->value == finish)
		{//top is finish, so we have found path
			backtraceTempNode = top;
			foundFinish = true;
			break;//we will need to trace back using preds later..
		}
		if(visited.count(top->value) == 0)//we havent visited top yet
		{
			visited.insert(top->value);
			expansionNum++;
			//add valid neighbors of top to PQ
			set<string> validNeighbors = getValidNeighbors(top,permittedNodes);
			set<string>::iterator it = validNeighbors.begin();
			for(it = validNeighbors.begin(); it != validNeighbors.end(); ++it)
			{
				Node* newnode = new Node();
				newnode->value = (*it);
				newnode->h = findH(newnode);
				newnode->g = top->g + 1;
				newnode->f = newnode->g + newnode->h;
				newnode->predecessor = top;
				priorityqueue.push(newnode);
				allDynamicallyAllocatedNodes.push_back(newnode);
			}
			
		}
	}
	if(foundFinish)//there is a path! 
	{//start backtracing at node with 111111 value
		vector<string> path;
		while(backtraceTempNode != NULL)
		{
			path.push_back(backtraceTempNode->value);
			backtraceTempNode = backtraceTempNode->predecessor;
		}
		for(unsigned int i = path.size()-1; i > 0; i--)
		{
			cout<<path[i]<<endl;
		}
		cout<<path[0]<<endl;

		cout<<"expansions = "<<expansionNum<<endl;
	}
	else//no path :(
	{
		cout<<"No transformation"<<endl;
		cout<<"expansions = "<<expansionNum<<endl;
	}
	//delete all the nodes now to deallocate
	for(unsigned int i = 0; i < allDynamicallyAllocatedNodes.size();i++)
	{
		delete allDynamicallyAllocatedNodes[i];
	}
	return 1;
}

int findH(const Node* node)
{
	string bits = node->value;
	int numzeros = 0;//count how many zeros are in node's bit string
	for(unsigned int i =0; i < bits.size(); i++)
	{
		if(bits[i] == '0')
			numzeros++;
	}
	return numzeros;
}
//find all the valid neighors of a given node, return a set that holds them
set<string> getValidNeighbors(Node* node, const vector<string> &permittedNodes)
{
	set<string> validNeighbors;
	string nodeVal = node->value;
	for(unsigned int i = 0; i < permittedNodes.size(); i++)
	{//loop through permitted nodes to see which bit strings can be neighbors
		int numUnmatchingBits = 0;
		string tempstr = permittedNodes[i];
		for(unsigned int j = 0; j < tempstr.size(); j++)
		{//loop through each bit in string of permitted and node
			if(tempstr[j] != nodeVal[j])
			{
				numUnmatchingBits++;
			}
		}
		if(numUnmatchingBits == 1)
		{//only one bit flip, so valid
			validNeighbors.insert(tempstr);
		}
	}
	return validNeighbors;
}