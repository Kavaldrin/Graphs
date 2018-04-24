
#include "Adjacency.h"
#include "Incidence.h"
#include "AdjacencyList.h"
#include <iostream>
#include <fstream>
#include <string>	

const std::string Adjacency_type = "type: Adjacenty";
const std::string Incidence_type = "type: Incidence";
const std::string AdjacencyList_type = "type: AdjacencyList";



int main()
{
	//opening file
	std::fstream file;
	file.open("data.txt", std::ios::in);
	if (!file.good()) {
		std::cout << "Cannot open the file\n";
		system("pause");
		return 0;
	}
	//////////////////////////// preparing file
	// for linux isues
	std::string type;
	std::string temp;
	file >> temp;
	type = temp;
	file >> temp;
	type += " " + temp;

	////
	file >> temp;
	unsigned size;
	file >> size;
	//////////////////////////////////////
	Representation *orginal_representation = nullptr;

	//creating proper object
	
	if (type == Adjacency_type)
		orginal_representation = new Adjacency(size, file);
	else if (type == Incidence_type) {
		int paths;
		file >> paths;
		orginal_representation = new Incidence(size,paths, file);
	}
	else if (type == AdjacencyList_type)
		orginal_representation = new AdjacencyList(size, file);
	else {
		std::cout << "Invalid file format.\n";
		return 0;
	}
	file.close();
	
	//converting
	Representation* converted = nullptr;
	Representation* converted2 = nullptr;

	orginal_representation->print();
	if (type == AdjacencyList_type) {
		converted = orginal_representation->convertToAdjacency();
		converted2 = orginal_representation->convertToIncidence();
	}
	else if (type == Adjacency_type) {
		converted = orginal_representation->convertToAdjacencyList();
		converted2 = orginal_representation->convertToIncidence();
	}
	else if (type == Incidence_type) {
		converted = orginal_representation->convertToAdjacencyList();
		converted2 = orginal_representation->convertToAdjacency();
	}


	converted->print();
	converted2->print();

	orginal_representation->saveToDraw();
	converted->saveToDraw();
	converted2->saveToDraw();

	orginal_representation->saveType();
	converted->saveType();
	converted2->saveType();

	delete converted;
	delete converted2;
	delete orginal_representation;

	system("pause");


}