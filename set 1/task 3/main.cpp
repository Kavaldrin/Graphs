#include <iostream>
#include <utility>
#include <ctime>
#include <vector>
#include <fstream>
#include <cmath>


void saveToFile(const int n, std::vector<std::pair<int, int>> &pairs)
{
	std::fstream file("saveToDraw.txt", std::ios::out);
	std::fstream file2("data.txt", std::ios::out);
	
	if (!file.good())
		return;
	file << n << " " << pairs.size() << std::endl;

	for (auto &pair : pairs)
		file << pair.first << " " << pair.second << std::endl;
	file.close();

	if (!file2.good())
		return;

	int ** matrix = new int*[n];
	for (unsigned i = 0; i < n; ++i)
		matrix[i] = new int[pairs.size()];
	for (unsigned i = 0; i < n; ++i)
		for (unsigned j = 0; j < pairs.size(); ++j)
			matrix[i][j] = 0;
		

	for (int i = 0; i < pairs.size(); ++i) {
		matrix[pairs[i].first - 1][i] = 1;
		matrix[pairs[i].second - 1][i] = 1;
	}

	file2 << "type: Incidence" << std::endl;
	file2 << "size: " << n << " " << pairs.size() << std::endl;

	for (unsigned i = 0; i < n; ++i) {
		for (unsigned j = 0; j < pairs.size(); ++j)
			file2 << matrix[i][j] << " ";
		file2 << std::endl;
	}
	
	file2.close();

	for (unsigned i = 0; i < n; ++i)
		delete[] matrix[i];
	delete[] matrix;


}


void GenerateNP(const int n, const double p)
{
	std::vector<std::pair<int, int>> pairs;

	for (unsigned i = 0; i < n; ++i) {
		for (unsigned j = i+1; j < n; ++j) {
			if ((rand() / static_cast<double>(RAND_MAX)) < p) {
				pairs.push_back(std::make_pair(i + 1, j + 1));
			}
		}
	}
	
	saveToFile(n, pairs);

}

void GenerateNL(const int n, int l)
{

	std::vector<std::pair<int, int>> pairs;

	for (unsigned i = 0; i < n && l; ++i) {
		for (unsigned j = i+1; j < n && l; ++j) {
			pairs.push_back(std::make_pair(i + 1, j + 1));
			--l;
		}
	}
	
	saveToFile(n, pairs);
}

int main()
{
	srand(time(NULL));
	std::cout << "1 dla generowaniu po prawodopodienstwie" << std::endl;
	std::cout << "2 dla generowaniu po ilosci drog" << std::endl;
	int choice;
	std::cin >> choice;
	while (choice != 1 && choice != 2) {
		std::cout << "1 dla generowaniu po prawodopodienstwie" << std::endl;
		std::cout << "2 dla generowaniu po ilosci drog" << std::endl;
		std::cin >> choice;
	}
	int n;
	std::cout << "Wpisz liczbe wierzcholkow" << std::endl;
	std::cin >> n; n = abs(n);

	if (choice == 1) {
		double d_choice;
		std::cout << "Wpisz liczbe prawdopodobienstwa (0,1)" << std::endl;
		std::cin >> d_choice;
		while (d_choice <= 0 || d_choice >= 1) {
			std::cout << "Wpisz liczbe prawdopodobienstwa (0,1)" << std::endl;
			std::cin >> d_choice;
		}
		GenerateNP(n, d_choice);
		
	}
	else if(choice == 2) {
		int i_choice;
		std::cout << "Wpisz liczbe drog miedzy wierzcholkami" << std::endl;
		std::cin >> i_choice; i_choice = abs(i_choice);
		GenerateNL(n, i_choice);
	}
	
	std::cout << "Graf zostal wygenerowany do pliku" << std::endl;
	system("pause");

	return 0;
}