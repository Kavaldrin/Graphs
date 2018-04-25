#include "Fun.h"



using namespace std;

int main()
{
	fstream file("input_data.txt", ios::in);
	if (!file.good()) {
		cout << "Cannot open the file\n";
		system("pause");
		return 0;
	}

	vector <int> sequence;
	vector <int> orginal_sequence;

	if (!prepareSequence(file, sequence,orginal_sequence)) {
		cout << "Cannot generate graph from that sequence(input)\n";
		system("pause");
		return 0;
	}


	cout << "Sequence: ";
	for (int &a : orginal_sequence)
		cout << a << " ";
	if (checkGraph(sequence))
		cout << "generates graph\n";
	else
		cout << "doesnt generate graph\n";


	vector<vector<int>> adjacencyList;
	generateGraph(orginal_sequence, adjacencyList);
	vector<pair<int, int>> pairs;
	setOrginalPairs(adjacencyList, pairs);

	for (auto & pair : pairs) {
		cout << pair.first << " " << pair.second << endl;
	}



	randomizeGraph(pairs);


	system("pause");
	return 0;
}