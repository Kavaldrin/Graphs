#include "Incidence.h"
#include <utility>

Incidence::Incidence(const unsigned size,const unsigned paths, std::fstream& file):Representation(size),m_paths(paths)
{
	m_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		m_matrix[i] = new int[paths];

	for (unsigned i = 0; i < size; ++i)
		for (unsigned j = 0; j < paths; ++j)
			file >> m_matrix[i][j];
}

Incidence::Incidence(const unsigned size,const unsigned paths, int ** matrix):Representation(size),m_paths(paths)
{
	m_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		m_matrix[i] = new int[m_paths];
	for (unsigned i = 0; i < size; ++i)
		for (unsigned j = 0; j < m_paths; ++j)
			m_matrix[i][j] = matrix[i][j];

}

Incidence::~Incidence()
{
	for (unsigned i = 0; i < m_size; ++i)
		delete[] m_matrix[i];
	delete[] m_matrix;
}

void Incidence::print() const
{
	std::cout << "Incidence matrix representation\n";
	printf("     ");
	for (unsigned i = 0; i < m_paths; ++i)
		printf(" e%d", i + 1);
	std::cout << std::endl;
	printf("     ");
	for (unsigned i = 0; i < m_paths; ++i)
		std::cout << "___";
	std::cout << std::endl;

	for (unsigned i = 0; i<m_size; ++i) {
		printf("%3d |", i + 1);
		for (unsigned j = 0; j < m_paths; ++j) {
			printf("%3d", m_matrix[i][j]);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

Representation* Incidence::convertToAdjacency()
{
	std::map <int, std::vector<int>> pairs;

	int **new_matrix = new int*[m_size];
	for (unsigned i = 0; i < m_size; ++i)
		new_matrix[i] = new int[m_size];
	
	for (unsigned i = 0; i < m_size; ++i)
		for (unsigned j = 0; j < m_size; ++j)
			new_matrix[i][j] = 0;

	// i wierzcholki
	// j sciezki
	

	//zamysl polega ogolnie na stworzeniu mapa, odwolujacego sie do sciezki
	//a nastepnie do id drogi, przypisujemy wierzcholki z ktorych sklada sie droga
	
	for (unsigned i = 0; i < m_size; ++i) {
		for (unsigned j = 0; j < m_paths; j++) {
			if (m_matrix[i][j]) {
				pairs[j+1].push_back(i+1);
			}
		}
	}
	//otrzymane key -> numer sciezki (polaczenie kolejne)
	//wierzcholki zawierajace sie w tym polaczeniu

	//second, chodzi o wektor z sasiadami
	//a tutaj uzupelniamy macierz sasiedztwa
	for (auto & x : pairs) {
		new_matrix[x.second[0] - 1][x.second[1] - 1] = 1;
		new_matrix[x.second[1] - 1][x.second[0] - 1] = 1;
	}

	Representation* representation = new Adjacency(m_size, new_matrix);
	
	for (unsigned i = 0; i < m_size; ++i)
		delete[] new_matrix[i];
	delete[] new_matrix;



	return representation;
}

Representation * Incidence::convertToIncidence()
{
	std::cout << "Cannot convert Incidence matrix to Incidence matrix representation\n";
	return this;
}

Representation * Incidence::convertToAdjacencyList()
{
	
	//j path
	//i vertex
	std::vector<std::vector <int>> pairs;
	std::vector<int> temp;
	for (unsigned i = 0; i < m_paths; ++i)
		pairs.push_back(temp);

	//podobna koncepcja co wyzej, tylko innym sposobem
	//j oznacza droge, a i to wierzcholek

	//dodaje sobie wszystkie drogi, jakie maja wierzcholki
	for (unsigned i = 0; i < m_size; ++i) {
		for (unsigned j = 0; j < m_paths; ++j) {
			if (m_matrix[i][j]) {
				pairs[j].push_back(i + 1);
			}
		}
	}

	

	std::vector<std::pair<int,int>> list;

	//a tutaj dla kazdej drogi tworze sasiada, nawzajem

	for (auto & pair : pairs) {
		list.push_back(std::make_pair(pair[0], pair[1]));
		list.push_back(std::make_pair(pair[1], pair[0]));
	}
	
	Representation* repre = new AdjacencyList(m_size, list);

	return repre;
}

