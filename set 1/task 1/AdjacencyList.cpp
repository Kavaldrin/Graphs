#include "AdjacencyList.h"

AdjacencyList::AdjacencyList(const unsigned size, std::fstream& file):Representation(size)
{

	std::vector <int> vec;
	for (unsigned i = 0; i < m_size; ++i)
		m_list.push_back(vec);

	int num_of_neighbours;
	int temp;
	char temp_str[16];

	for (unsigned i = 0; i < m_size; ++i) {
		file >> num_of_neighbours;
		file >> temp_str;
		for (unsigned j = 0; j < num_of_neighbours; ++j) {
			file >> temp;
			m_list[i].push_back(temp);
		}
	}
}

AdjacencyList::AdjacencyList(const unsigned size, std::vector<std::pair<int, int>>& pairs):Representation(size)
{
	std::vector <int> vec;
	for (unsigned i = 0; i < m_size; ++i)
		m_list.push_back(vec);

	for (auto & pair : pairs) {
		m_list[pair.first-1].push_back(pair.second);
	}

}

Representation * AdjacencyList::convertToAdjacencyList()
{
	std::cout << "Cannot convert Adjacency List to Adjacency List\n";
	return this;
}

void AdjacencyList::saveToDraw() const
{
	//ogolnie ta metoda powina byc w incidence
	//ale juz mi sie nie chce zmieniac XD
	//bo tam sa juz gotowe unikalne pary

	std::vector< std::pair<int, int> > pairs;
	const int numVertex = m_list.size();

	int iterator = 1;
	for (auto & vertex_neigh : m_list) {
		for (auto vertex : vertex_neigh) {
			pairs.push_back(std::make_pair(iterator, vertex));
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

	std::fstream file("saveToDraw.txt", std::ios::out);
	if (!file.good())
		return;

	file << numVertex << " ";
	file << pairs.size() << std::endl;

	for (auto & pair : pairs) {
		file << pair.first << " " << pair.second << std::endl;
	}

	file.close();


}

void AdjacencyList::saveType() const
{
	using namespace std;

	fstream file("data_adjacencylist.txt", ios::out);
	file << "type: AdjacencyList"<<endl;
	file << "size: " << m_size << endl;

	int iterator = 0;
	for (auto & vertex : m_list) {
		file << vertex.size() << " " << iterator + 1 << ":";
		for (auto &neigh : vertex) {
			file << " " << neigh;
		}
		++iterator;
		file << endl;
	}
	file.close();
}

void AdjacencyList::print() const
{
	std::cout << "Adjacency List representation\n";
	unsigned index = 0;
	for (auto &vertex : m_list) {
		std::cout << index+1 << ": ";
		for (const int num : vertex) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
		++index;
	}
	std::cout << std::endl;
}

Representation * AdjacencyList::convertToAdjacency()
{
	int** new_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		new_matrix[i] = new int[m_size];

	for (unsigned i = 0; i < m_size; ++i)
		for (unsigned j = 0; j < m_size; ++j)
			new_matrix[i][j] = 0;

	int vertex = 0;
	for (auto& vec : m_list) {
		for (const int neighbour : vec) {
			new_matrix[neighbour - 1][vertex] = 1;
			new_matrix[vertex][neighbour - 1] = 1;
		}
		++vertex;
	}

	Representation* repre = new Adjacency(m_size, new_matrix);

	for (unsigned i = 0; i < m_size; ++i)
		delete[] new_matrix[i];
	delete[] new_matrix;
	return repre;

}

Representation * AdjacencyList::convertToIncidence()
{
	

	std::vector<std::pair<int,int>> pairs;

	int numVertex = 0;
	for (auto &neighbours : m_list) {
		for (const int num : neighbours) {
			pairs.push_back(std::make_pair(numVertex + 1, num));
		}
		++numVertex;
	}

	for (unsigned i = 0; i < pairs.size(); ++i) {
		for (unsigned j = i + 1; j < pairs.size(); ++j) {
			if ((pairs[i].first == pairs[j].second) && (pairs[i].second == pairs[j].first)) {
				pairs[j] = pairs[pairs.size() - 1];
				pairs.pop_back();
			}
		}
	}
	int paths = pairs.size();
	int **new_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		new_matrix[i] = new int[paths];

	for (unsigned i = 0; i < m_size; ++i)
		for (unsigned j = 0; j < paths; ++j)
			new_matrix[i][j] = 0;

	int way_counter = 0;
	for (auto &pair : pairs) {
		new_matrix[pair.first - 1][way_counter] = 1;
		new_matrix[pair.second - 1][way_counter] = 1;
		++way_counter;
	}

	Representation * repre = new Incidence(m_size,paths, new_matrix);
	
	for (unsigned i = 0; i < m_size; ++i)
		delete[] new_matrix[i];
	delete[] new_matrix;

	return repre;

}
