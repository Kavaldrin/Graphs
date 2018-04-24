#pragma once
#include "Representation.h"
#include "Incidence.h"
#include "AdjacencyList.h"

class Adjacency :
	public Representation
{

public:
	Adjacency(const unsigned size, int **matrix);
	Adjacency(const unsigned size, std::fstream& file);
	~Adjacency();
	Representation* convertToAdjacency();
	Representation* convertToIncidence();
	Representation* convertToAdjacencyList();
	virtual void saveToDraw() const{}
	void saveType() const {} // nie chce mi sie
	void print() const;
protected:
	int **m_matrix;
};

