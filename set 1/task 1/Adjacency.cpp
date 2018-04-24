#include "Adjacency.h"



Adjacency::Adjacency(const unsigned size, std::fstream &file):Representation(size)
{
	m_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		m_matrix[i] = new int[m_size];

	for (unsigned i = 0; i < size; ++i) 
		for (unsigned j = 0; j < size; ++j) 
			file >> m_matrix[i][j];
}

Adjacency::~Adjacency()
{
	for (unsigned i = 0; i < m_size; ++i)
		delete[] m_matrix[i];
	delete[] m_matrix;
}


void Adjacency::print() const
{
	std::cout << "Adjacency matrix representation\n";
	printf("     ");
	for (unsigned i = 0; i < m_size; ++i)
		printf("%3d", i+1);
	std::cout << std::endl;
	printf("     ");
	for (unsigned i = 0; i < m_size; ++i)
		std::cout << "___";
	std::cout << std::endl;

	for (unsigned i = 0; i<m_size; ++i) {
		printf("%3d |", i + 1);
		for (unsigned j = 0; j < m_size; ++j) {
			printf("%3d", m_matrix[i][j]);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
Adjacency::Adjacency(const unsigned size, int **matrix):Representation(size)
{
	m_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		m_matrix[i] = new int[m_size];

	for (unsigned i = 0; i < size; ++i)
		for (unsigned j = 0; j < size; ++j)
			m_matrix[i][j] = matrix[i][j];
}
Representation* Adjacency::convertToAdjacency()
{
	std::cout << "Cannot convert Adjacency matrix to Adjacency matrix representation\n";
	return this;
}

Representation * Adjacency::convertToIncidence()
{
	

	std::vector<std::pair<int,int>> pairs;
	//dodawanie par
	for (unsigned i = 0; i < m_size; ++i) {
		for (unsigned j = 0; j < m_size; ++j) {
			if (m_matrix[i][j]) {
				pairs.push_back(std::make_pair(i+1, j+1));
			}
		}
	}
	//usuwanie zamiennych typu 1 - 3, 3 - 1
	for (unsigned i = 0; i < pairs.size(); ++i) {
		for (unsigned j = i + 1; j < pairs.size(); ++j) {
			if ((pairs[i].first == pairs[j].second) && (pairs[i].second == pairs[j].first)) {
				pairs[j] = pairs[pairs.size() - 1];
				pairs.pop_back();
			}
		}
	}

	//przypisanie unikalnej liczby drog
	int paths = pairs.size();

	int **new_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		new_matrix[i] = new int[paths];

	for (unsigned i = 0; i < m_size; ++i)
		for (unsigned j = 0; j < paths; ++j)
			new_matrix[i][j] = 0;

	//utworzenie macierzy
	int way_counter = 0;
	for (auto &pair : pairs) {
		new_matrix[pair.first-1][way_counter] = 1;
		new_matrix[pair.second-1][way_counter] = 1;
		++way_counter;
	}

	//przekonwertowanie do tej incydencji
	Representation * representation = new Incidence(m_size,paths, new_matrix);
	
	for (unsigned i = 0; i < m_size; ++i)
		delete[] new_matrix[i];
	delete[] new_matrix;

	return representation;
	
}

Representation * Adjacency::convertToAdjacencyList()
{

	std::vector<std::pair<int,int>> pairs;
	//stworzenie par
	for (unsigned i = 0; i < m_size; ++i) {
		for (unsigned j = 0; j < m_size; ++j) {
			if (m_matrix[i][j]) {
				pairs.push_back(std::make_pair(i + 1, j + 1));
			}
		}
	}
	//przekazanie wektora par do listy sasiedztwa, stworzenie w jego konstruktorze
	Representation *rep = new AdjacencyList(m_size, pairs);

	return rep;
}
