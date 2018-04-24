#include "Graph.h"



void Graph::testPrint() const
{
	for (auto & vertex : vertexes) {
		std::cout << vertex.m_index+1 << "("<<vertex.m_group<<"):";
		for (auto &neigh : vertex.m_connections) {
			std::cout << " " << neigh+1;
		}
		std::cout << std::endl;
	}
}

void Graph::groupComponents()
{
	int number = 0;

	for (Vertex &v : vertexes) {
		if (!v.m_visited) {
			++number;
			v.m_group = number;
			v.m_visited = true;
			componentsR(number, v);
		}
	}

	m_connectivity = true;
	for (Vertex &v : vertexes) {
		if (v.m_group > 1) {
			m_connectivity = false;
			break;
		}
	}
}

void Graph::componentsR(char group, Vertex & v)
{
	for (int neigh : v.m_connections) {
		if (!vertexes[neigh].m_visited) {
			vertexes[neigh].m_visited = true;
			vertexes[neigh].m_group = group;
			componentsR(group, vertexes[neigh]);
		}
	}
}

void Graph::resetVisited()
{
	for (auto &vertex : vertexes) vertex.m_visited = false;
}

std::vector<int> Graph::findLargestGroup()
{
	//memory wasting style =D
	const int SIZE = vertexes.size();
	int *array = new int[SIZE];
	for (int i = 0; i < SIZE; ++i) array[i] = 0;

	for (auto &vertex : vertexes) ++array[vertex.m_group - 1];

	std::vector<int> maxes;
	maxes.push_back(0);

	for (int i = 1; i < SIZE; ++i) {
		if (array[i] > array[maxes[0]]) {
			maxes.clear();
			maxes.push_back(i);
		}
		else if (array[i] == array[maxes[0]])
			maxes.push_back(i);
	}
	return maxes;


}

bool Graph::eulerPossibility()
{
	for (auto &vertex : vertexes) {
		if (vertex.m_connections.size() % 2)
			return false;
	}
	return true;
}

std::vector<int> Graph::generateEuler()
{
	//preparing and making sure that can be euler cycle
	while (!m_connectivity) {
		resetVisited();
		randomizeGraph();
		groupComponents();
		throw myException("cannot make connectivity\n -> cannot generate Euler cycle");
	}

	for (auto &vertex : vertexes) {
		std::sort(vertex.m_connections.begin(), vertex.m_connections.end());
	}

	Graph copy(*this);
	std::vector<int> path;
	std::stack<int> s;
	std::map<int, int> count_egdes;
	int iter = 0;

	for (auto &vertex : copy.vertexes) {
		count_egdes[iter] = vertex.m_connections.size();
		++iter;
	}

	////////////
	//http://www.graph-magics.com/articles/euler.php

	int curr_v = 0;
	s.push(0); //poczatek drogi

	while (!s.empty()) {
		if (count_egdes[curr_v]) {
			int new_v = copy.vertexes[curr_v].m_connections[0];
			--count_egdes[curr_v];
			--count_egdes[new_v];

			copy.disconnect(new_v, curr_v);

			s.push(curr_v);
			curr_v = new_v;
		}
		else {
			path.push_back(curr_v);
			curr_v = s.top();
			s.pop();
		}

	}

	return path;
}

bool foundHamilton = false;

std::vector<int> Graph::generateHamilton()
{

	int rand_attempts = 10000;

	groupComponents();
	while (!m_connectivity) {
		resetVisited();
		randomizeGraph();
		groupComponents();
		--rand_attempts;
		if (!rand_attempts) {
			throw myException("cannot make connectivity\n -> cannot generate Hamilton cycle");
		}
	}
	resetVisited();

	std::vector<int> s;
	int v = 0;

	s.push_back(v);
	vertexes[v].m_visited = true;
	while ((!foundHamilton) && (!HamiltonNext(v, s)) && (v < (vertexes.size() - 1))) {
		vertexes[v].m_visited = false;
		++v;
		s.pop_back();
		s.push_back(v);
	}

	if (foundHamilton)
		s.push_back(v);
	else
		s.pop_back();


	if (foundHamilton)
		std::cout << "Found cycle: \n";
	else
		std::cout << "Didnt find cycle \n";
//#define DEBUG
#ifdef DEBUG
	for (auto &a : s)
		std::cout << a + 1 << " ";
	std::cout << std:: endl;
#endif DEBUG

	return s;

}

bool Graph::HamiltonNext(int v, std::vector<int> &s)
{
	if (foundHamilton)
		return true;
	else if (s.size() == vertexes.size() && uniq_elems(s)) {
		if (areConnected(v,s[0])) {
			foundHamilton = true;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		for (int i = 0; i < vertexes[v].m_connections.size(); ++i) {
			if (!vertexes[vertexes[v].m_connections[i]].m_visited) {
				s.push_back(vertexes[v].m_connections[i]);
				vertexes[vertexes[v].m_connections[i]].m_visited = true;
				if (HamiltonNext(vertexes[v].m_connections[i], s))
					return true;

#ifdef DEBUG

				for (auto &a : s)
					std::cout << a +1 << " ";
				std::cout << std::endl;
#endif
				s.pop_back();
				vertexes[vertexes[v].m_connections[i]].m_visited = false;
			}
		}
		return false;
	}
	
}
void Graph::onlyComponents()
{
	std::cout << "Without groups: \n";
	testPrint();
	groupComponents();
	std::cout << "After grouping components: \n";
	testPrint();
	std::vector<int> largest = findLargestGroup();
	std::cout << "Largest groups:";
	for (auto &l : largest) std::cout << " " << l + 1;
	std::cout << std::endl;

}
bool Graph::uniq_elems(std::vector<int> &s)
{
	for (int i = 1; i < s.size(); ++i) {
		for (int j = 0; j < i; ++j) {
			if (s[i] == s[j])
				return false;
		}
	}
	return true;
}


void Graph::randomizeGraph()
{
	using namespace std;
	const int vertex_num = vertexes.size();

	srand(time(NULL));
	int a, b, c, d, i;
	//ab i cd na parê ad i cb.
	while (true) {
		a = std::rand() % vertex_num;
		b = std::rand() % vertex_num;
		c = std::rand() % vertex_num;
		d = std::rand() % vertex_num;
		if (a != b && a != c && a != d && b != c && b != d && c != d) {
			if ((areConnected(a,b) && areConnected(c,d) && !areConnected(a,d) && !areConnected(c, b))) {
				//std::cout << "debug : randoming" <<a+1 << " "<<b+1<< " " << c+1 << " "<<d+1<< std::endl;
				disconnect(a, b);
				disconnect(c, d);
				connect(a, d);
				connect(c, b);
				break;
			}
		}
	}
}



void Graph::saveToDraw(std::string type, std::vector<int>& path)
{
	using namespace std;
	fstream file("saveToDraw.txt", ios::out);


	//////////////////////////////////////////////////////////
	//copy paste from previous programms
	std::vector< std::pair<int, int> > pairs;
	const int numVertex = vertexes.size();

	int iterator = 0;
	for (auto & vertex : vertexes) {
		for (auto &c : vertex.m_connections) {
			pairs.push_back(std::make_pair(iterator, c));
		}
		++iterator;
	}

	//deleting copies

	for (unsigned i = 0; i < pairs.size(); ++i) {
		for (unsigned j = i + 1; j < pairs.size(); ++j) {
			if (pairs[j].first == pairs[i].second && pairs[j].second == pairs[i].first) {
				pairs[j] = pairs[pairs.size() - 1];
				pairs.pop_back();
			}
		}
	}

	///////////////////////////////////////////////////


	file << ("type: " + type) << endl;
	for (auto it = path.begin(); it != path.end(); ++it) {
		file << *it + 1 << " ";
	}
	file << endl;
	file << vertexes.size() << " " << pairs.size() << endl;
	for (auto & pair : pairs) {
		file << pair.first +1  << " " << pair.second +1 << std::endl;
	}




	file.close();
}
void Graph::EulerMode()
{
	using namespace std;

	testPrint();
	groupComponents();
	cout << "With groups" << endl;
	testPrint();
	std::vector<int> largest = findLargestGroup();
	cout << "Largest groups:";
	for (auto &l : largest) cout << " " << l + 1;
	cout << endl;

	if (!eulerPossibility()) {
		cout << "Cannot make euler graph from this input, every vertex MUST have even degree\n";
		return;
	}
	vector<int> euler_path = generateEuler();

	saveToDraw("Euler",euler_path);

	cout << "Eulers cycle: ";
	for (int &a : euler_path) {
		cout << a << " ";
	}
	cout << endl;
}

void Graph::HamiltonMode()
{
	using namespace std;

	testPrint();
	groupComponents();
	cout << "With groups" << endl;
	testPrint();
	std::vector<int> largest = findLargestGroup();
	cout << "Largest groups:";
	for (auto &l : largest) cout << " " << l + 1;
	cout << endl;

	
	vector<int> hamilton_path = generateHamilton();

	saveToDraw("Hamilton",hamilton_path);

	cout << "Hamiltons cycle: ";
	for (int &a : hamilton_path) {
		cout << a << " ";
	}
	cout << endl;
}



bool Graph::areConnected(int v1, int v2)
{
	for (int &neigh : vertexes[v1].m_connections) {
		if (neigh == v2)
			return true;
	}

	return false;
}

void Graph::disconnect(int v1, int v2)
{
	auto it = std::find(vertexes[v1].m_connections.begin(), vertexes[v1].m_connections.end(), v2);
	vertexes[v1].m_connections.erase(it);
	it = std::find(vertexes[v2].m_connections.begin(), vertexes[v2].m_connections.end(), v1);
	vertexes[v2].m_connections.erase(it);
}

void Graph::connect(int v1, int v2)
{
	vertexes[v1].m_connections.push_back(v2);
	vertexes[v2].m_connections.push_back(v1);
}
