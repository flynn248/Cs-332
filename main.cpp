//Developed by Shane Flynn
//Completed using MS Visual Studio 2019
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <queue>

using namespace std;
 
void openFileCheck(fstream&);
void displayMenu(string*, double**, int&);
int findIndex(string*, int&, string&);
bool depthFirstSearch(int , int, bool*, int& size, double**);
bool breadthFirstSearch(int, int, int&, double**);
void isEdge(double**, int&, int&);

int main()
{
	fstream readLabels("labels.csv", ios::in);
	string temp; //temp value to hold input from file
	int size = 0; //to hold size of array

	openFileCheck(readLabels);

	while (getline(readLabels, temp, ',')) //purpose is to find the total # of nodes in graph
	{
		size++;
		if (readLabels.eof()) break;
	}

	string* labelArray = new string[size]; //to hold location names

	readLabels.seekg(0); //move cursor to beginning of file

	for (int i = 0; i < size; i++) //Parse the location values from file into labelArray
	{
		if (i != size-1)
			getline(readLabels, labelArray[i], ',');
		else
			getline(readLabels, labelArray[i], '\n');
	}
	
	readLabels.close();

	double** edgeArray = new double*[size]; //array to hold edge weights

	for (int i = 0; i < size; i++) //making array 2D
	{
		edgeArray[i] = new double[size];
	}

	fstream readEdge("distances.csv", ios::in);
	
	for (int i = 0; i < size; i++) //parse distances into array
	{
		for (int j = 0; j < size; j++)
		{
			if (j != size-1) //Prevents a bug from reading the '\n' as an input
			{
				getline(readEdge, temp, ',');
				edgeArray[i][j] = stod(temp);
			}
			else
			{
				getline(readEdge, temp, '\n');
				edgeArray[i][j] = stod(temp);
			}
		}
	}

	readEdge.close();

	displayMenu(labelArray, edgeArray, size);

	for (int i = 0; i < size; i++) //deleting row of pointers
	{
		delete [] edgeArray[i];
	}

	delete [] edgeArray;
	delete [] labelArray;
	labelArray = nullptr;
	edgeArray = nullptr;
	return 0;
}

void openFileCheck(fstream &file) //check if file failed to open
{
	if (!file)
	{
		cout << "File failed to open..." << endl;
		cout << "Terminating program..." << endl;
		cout << "Hit enter to close window...   " << endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
}

void displayMenu(string* labelA, double** edgeA, int& size) //display stuff to user
{
	string node1,
		node2,
		path;
	int node_1_index,
		node_2_index;

	bool* discovered = new bool[size]();

	cout << "Necessary files have been read!\n" << endl;

	cout << "Enter City #1: ";
	getline(cin, node1);
	cout << "Enter City #2: ";
	getline(cin, node2);

	node_1_index = findIndex(labelA, size, node1);
	node_2_index = findIndex(labelA, size, node2);

	cout << "\nIndex of " << node1 << " is: " << node_1_index << endl;
	cout << "Index of " << node2 << " is: " << node_2_index << endl << endl;

	isEdge(edgeA, node_1_index, node_2_index);

	if (depthFirstSearch(node_1_index, node_2_index, discovered, size, edgeA))
		path = "TRUE!";
	else
		path = "FALSE!";

	cout << "DFS Path Exists: " << path << endl;

	if (breadthFirstSearch(node_1_index, node_2_index, size, edgeA))
		path = "TRUE!";
	else
		path = "NEGATORY!";
	
	cout << "BFS Path Exists: " << path << endl;
}

int findIndex(string* labelA, int& size, string& nodeName) //finds index of user input label
{
	for (int i = 0; i < size; i++)
		if (nodeName == labelA[i])
			return i;
}

void isEdge(double** edgeA, int& nodeA, int& nodeB) //check for existance of an edge between two nodes
{
	if (edgeA[nodeA][nodeB] != -1) {
		cout << "An edge exists!\n";
		cout << "Distance: " << edgeA[nodeA][nodeB] << " mi.\n\n";
	}
	else
		cout << "There is no edge between them!\n\n";
}

bool depthFirstSearch(int source, int target, bool* discovered, int& size, double ** edgeA) { //recursive version
	discovered[source] = true;

	if (source == target)
		return true;

	for (int neighbor = 0; neighbor < size; neighbor++)
	{
		if (edgeA[source][neighbor] != -1) //find neighbors
		{
			if (discovered[neighbor] == true)
				continue;

			if (depthFirstSearch(neighbor, target, discovered, size, edgeA))
				return true;		
		}
	}

	return false;
}

bool breadthFirstSearch(int source, int target, int& size, double ** edgeA)
{
	bool* discovered = new bool[size]();

	queue<int> que;

	que.push(source);
	discovered[source] = true;

	while (que.empty() == false)
	{
		int current = que.front();
		que.pop();

		if (current == target)
			return true;

		for (int neighbor = 0; neighbor < size; neighbor++)
		{
			if (edgeA[current][neighbor] != -1) //find neighbors
			{
				if (discovered[neighbor] == false)
				{
					discovered[neighbor] = true;
					que.push(neighbor);
				}
			}
		}
	}
	
	return false;
}