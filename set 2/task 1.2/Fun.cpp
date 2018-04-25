
#include "Fun.h"

int file_iterator = 0;
int vertex_num;

bool prepareSequence(std::fstream &file, std::vector <int> &sequence, std::vector <int> &orginal)
{
	using namespace std;

	string temp;
	getline(file, temp);
	file.close();
	if (temp[0] > '9' && temp[0] < '0')
		return false;

	int numbers = 1;
	int it = 0;
	while (temp[it]) {
		if (temp[it] == ' ')
			++numbers;
		++it;
	}

	stringstream ss(temp);
	int itemp;
	for (int i = 0; i < numbers; ++i) {
		ss >> itemp;
		sequence.push_back(itemp);
	}
	
	int odds = 0;
	int max = sequence[0];

	for (int &a : sequence) {
		if (a % 2)
			++odds;
		if (a > max)
			max = a;
	}
	if ((odds % 2) || max >= numbers) 
		return false;
	

	orginal = sequence;
	vertex_num = orginal.size();
	sort(sequence.begin(), sequence.end(), myCompare);
	return true;

}

bool myCompare(int x, int y) { return x > y; }

bool checkGraph(std::vector <int> &sequence)
{
	int vertex;
	bool result;
	while (true) {

		//checking all elements = 0
		result = true;
		for (int &a : sequence) {
			if (a)
				result = false;
		}
		if (result)
			return true;
		/////

		vertex = sequence[0];
		if (vertex >= sequence.size())
return false;

for (int i = 1; i <= vertex; ++i)
--sequence[i];

sequence.erase(sequence.begin());
sort(sequence.begin(), sequence.end(), myCompare);
	}

	return true;
}

void generateGraph(std::vector<int> &orginal, std::vector<std::vector<int>> &adjacencyList)
{
	using namespace std;

	vector<int> temp;
	for (int i = 0; i < orginal.size(); ++i)adjacencyList.push_back(temp);

	vector<pair<int, int>> vertexes;
	for (int i = 0; i < orginal.size(); ++i)
		vertexes.push_back(make_pair(i, orginal[i]));

	sort(vertexes.begin(), vertexes.end(), comparePair);
	while (vertexes[0].second > 0) {

		for (int i = 1; i <= vertexes[0].second; ++i) {
			--vertexes[i].second;
			adjacencyList[vertexes[0].first].push_back(vertexes[i].first);
			adjacencyList[vertexes[i].first].push_back(vertexes[0].first);
		}
		vertexes[0].second = 0;
		sort(vertexes.begin(), vertexes.end(), comparePair);

	}

	//generating data.txt to graphic interpretation
	fstream file("data.txt", ios::out);
	if (!file.good()) {
		std::cout << "Cannot open/save to file...\n";
		return;
	}
	file << "type: AdjacencyList\n";
	file << "Size: " << adjacencyList.size() << endl;

	for (int i = 0; i < adjacencyList.size(); ++i) {
		file << adjacencyList[i].size() << " " << i + 1 << ":";
		for (int j = 0; j < adjacencyList[i].size(); ++j) {
			file << " " << adjacencyList[i][j] + 1;
		}
		file << endl;
	}
	file.close();

}

bool comparePair(std::pair<int, int> p1, std::pair<int, int> p2)
{

	return p1.second > p2.second;
}

void randomizeGraph(std::vector<std::pair<int, int>> &pairs)
{


	using namespace std;


	srand(time(NULL));
	int trials = 500000;
	int a, b, c, d, i;
	//ab i cd na parê ad i cb.
	while (trials) {
		a = std::rand() % vertex_num;
		b = std::rand() % vertex_num;
		c = std::rand() % vertex_num;
		d = std::rand() % vertex_num;
		if (a!= b && a!= c && a!= d && b!=c && b!=d && c!=d) {
			if (findPair(pairs, a, b) && findPair(pairs, c, d)
				&& !findPair(pairs, a, d) && !findPair(pairs, c, b)) {
				deletePair(pairs, a, b);
				deletePair(pairs, c, d);
				pairs.push_back(std::make_pair(a, d));
				pairs.push_back(std::make_pair(b, c));
			}
		}


		--trials;
	}


	//saving to file as incidence matrix
	 

	std::string path = "data" + to_string(file_iterator) + ".txt";
	std::fstream file(path, std::ios::out);
	if (!file.good()) {
		std::cout << "error with saving generated graph\n";
		return;
	}

	int **matrix = new int*[vertex_num];
	for (int i = 0; i < vertex_num; ++i) matrix[i] = new int[pairs.size()];
	for (int i = 0; i < vertex_num; ++i)
		for (int j = 0; j < pairs.size(); ++j)
			matrix[i][j] = 0;

	int iter = 0;
	for (auto &pair : pairs) {
		matrix[pair.first][iter] = 1;
		matrix[pair.second][iter] = 1;
		++iter;
	}



	file << "type: Incidence\n";
	file << "size: " << vertex_num << " " << pairs.size() << endl;
	for (int i = 0; i < vertex_num; ++i) {
		for (int j = 0; j < pairs.size(); ++j)
			file << matrix[i][j] << " ";
		file << std::endl;
	}

	for (int i = 0; i < vertex_num; ++i)
		delete[] matrix[i];
	delete[] matrix;

	file.close();
	++file_iterator;
	
}

void setOrginalPairs(std::vector<std::vector<int>> &adjacencyList, std::vector<std::pair<int, int>> &pairs)
{
	using namespace std;
	for (int i = 0; i < adjacencyList.size(); ++i) {
		for (int j = 0; j < adjacencyList[i].size(); ++j) {
			pairs.push_back(make_pair(i, adjacencyList[i][j]));
		}
	}

	//deleting duplicats

	for (int i = 0; i < pairs.size()-1; ++i) {
		for (int j = i + 1; j < pairs.size(); ++j) {
			if (pairs[i].first == pairs[j].second && pairs[i].second == pairs[j].first) {
				pairs[j] = pairs[pairs.size() - 1];
				pairs.pop_back();
				break;
			}
		}
	}
}

bool findPair(std::vector<std::pair<int, int>> &pairs,int x,int y)
{
	std::pair<int, int> pair1 = std::make_pair(x, y);
	std::pair<int, int> pair2 = std::make_pair(y, x);
	for (auto &par : pairs) {
		if (par == pair1 || par == pair2)
			return true;
	}
	return false;
}
void deletePair(std::vector<std::pair<int, int>> &pairs, int x, int y)
{
	std::pair<int, int> pair1 = std::make_pair(x, y);
	std::pair<int, int> pair2 = std::make_pair(y, x);
	for (auto it = pairs.begin();it!= pairs.end();++it) {
		if (*it == pair1 || *it == pair2) {
			pairs.erase(it);
			break;
		}
	}
}