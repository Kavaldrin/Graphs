#include "Fun.h"



int lowestBucket(std::vector<std::vector<int>> &bucket)
{
	for (int i = 0; i < bucket.size(); ++i) {
		if (bucket[i].size() > 0)
			return i;
	}
	throw std::exception();
}
bool areConnected(std::vector<std::vector<int>> &graph, int v1, int v2)
{
	for (auto &n : graph[v1]) {
		if (n == v2)
			return true;
	}
	return false;
}
void connect(std::vector<std::vector<int>> &graph, int v1, int v2)
{
	graph[v1].push_back(v2);
	graph[v2].push_back(v1);
}

void generateGraphFullRandom(int r, int w)
{
	using namespace std;
	srand(time(NULL));


	vector<int> points;
	vector<vector<int>> buckets;
	vector<vector<int>> graph;
	map<int, int> degrees;

	////////
	for (int i = 0; i < w; ++i)
		degrees[i] = 0;

	for (int i = 0; i < w; ++i) {
		graph.push_back(points);
	}
	for (int i = 0; i <= r; ++i)
		buckets.push_back(points);

	//////// w koszyczku z 0 stopniem wrzucamy wszystkie wierzcholki
	for (int i = 0; i < w; ++i)
		buckets[0].push_back(i);


	int v1, v2, temp, index, l;
	//dopoki wszystkie wierzcholki nie znajda sie w koszyczku z r stopniem jedziemy z koksem
	while (buckets[r].size() < w) {
		temp = rand() % r; // losujemy stopien dla wierzcholka
		while (buckets[temp].empty())
			temp = rand() % r;
		index = rand() % buckets[temp].size(); //losujemy index w wylosowanym stopniu
		v1 = buckets[temp][index]; //wpisujemy numer wierzcholka do v1
		buckets[temp].erase(buckets[temp].begin() + index); //usuwamy go z kubeczka z danym stopniem

		l = r - degrees[v1]; //??? losujemy mu tyle sasiadow ile mu brakuje zeby byc w r-1

							 //uzupelniamy mu sasiadow
		for (int i = 0; i < l; ++i) {
			temp = lowestBucket(buckets);
			index = rand() % buckets[temp].size();
			v2 = buckets[temp][index];
			while (!((v1 != v2) && !areConnected(graph, v1, v2))) {
				temp = lowestBucket(buckets);
				index = rand() % buckets[temp].size();
				v2 = buckets[temp][index];
			}
			connect(graph, v1, v2);

			++degrees[v2];
			buckets[temp].erase(buckets[temp].begin() + index);
			buckets[temp + 1].push_back(v2);
			//poprawny wierzcholek v2
		}
		//wrzucenie do finalnego...
		degrees[v1] = r;
		buckets[r].push_back(v1);
	}



	for (int i = 0; i < w; ++i) {
		cout << i+1 << ":";
		for (auto &n : graph[i])
			cout << " " << n+1;
		cout << endl;
	}
	
	


	try {
		saveToFile(graph);
	}
	catch (std::string str) {
		cout << str;
	}
	

}


void saveToFile(std::vector<std::vector<int>> &vertexes)
{
	using namespace std;
	fstream file("data.txt", ios::out);
	if (!file.good())
		throw "Cannot open the file\n";

	file << "type: AdjacencyList" << endl;
	file << "size: " << vertexes.size() << endl;

	int iter = 1;
	for (auto &vertex : vertexes) {
		file << vertex.size() << " " << iter << ":";
		++iter;
		for (auto &n : vertex) {
			file << " " << n+1;
		}
		file << endl;
	}



	file.close();
}