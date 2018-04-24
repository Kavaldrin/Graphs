#include "InputParser.h"


InputParser::InputParser(std::string fileName)
{
	using namespace std;
	
	try {
		file.open(fileName,ios::in);
		if (!file.good())
			throw myException("Cannot open file" + fileName);
	}
	catch(myException e){
		//std::cout << e.what() << std::endl;
		throw;
	}
	ptr = shared_ptr<Graph>(new Graph());

	std::string temp;
	int itemp;
	std::getline(file, temp); //skiping 2 first lines, wanna make suitable input for all programms
	if (temp != "type: AdjacencyList") {
		file.close();
		throw myException("Wrong file input in file: " + fileName + "\nShould be: \"type: AdjacencyList\""
			+ "\nYours is \"" + temp + "\"\n");
	}

	std::getline(file, temp); //and im too lazy to edit previous ones // working input without editing previous programm
	std::stringstream ss;
	std::string stemp;

	int vertex_num;

	while (std::getline(file, temp)) {

		Vertex vertex;
		ss.str(temp);

		ss >> itemp; //again im too lazy
		ss >> stemp;
		vertex_num = stoi(stemp);

		while (ss >> itemp) {
			if (itemp == vertex_num) {
				throw myException("Wrong input, vertex num = vertex neighbour");
			}

			vertex.addNeighbour(itemp-1); // -1 cuz of input file
		}
		vertex.setIndex(vertex_num-1); // -1 cuz of input file
		vertex.sortNeighbours();
		ptr->addVertex(vertex); //adding new vertex to graph
		ss.clear();
	}

	//checking connections
	std::vector <Vertex> testing = ptr->getVertexes();
	std::vector<int>::iterator it;
	for (Vertex &v : testing) {
		std::vector<int> neighbours = v.getNeighbours();
		for (int &n : neighbours){
			it = std::find(testing[n].getNeighbours().begin(), testing[n].getNeighbours().end(), v.getIndex());
			if (it == testing[n].getNeighbours().end())
				throw myException("Mistake in input...\n");
		}
	}

}


InputParser::~InputParser(){}
