#pragma once
#include "Representation.h"
#include "Adjacency.h"
class Incidence :
	public Representation
{

public:
	Incidence(const unsigned size,const unsigned paths, std::fstream& file);
	Incidence(const unsigned size,const unsigned paths, int **matrix);
	~Incidence();
	void print() const;
	Representation* convertToAdjacency();
	Representation* convertToIncidence();
	Representation* convertToAdjacencyList();
	virtual void saveToDraw() const{}
	void saveType() const {} // nie chce mi sie tego pisac

protected:
	int **m_matrix;
	unsigned m_paths;
};

