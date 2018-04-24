#pragma once
#include "Representation.h"
#include "Adjacency.h"
#include "Incidence.h"


class AdjacencyList :
	public Representation
{
public:
	AdjacencyList(const unsigned size,std::fstream& file);
	AdjacencyList(const unsigned size, std::vector<std::pair<int,int>>& pairs);
	~AdjacencyList(){}
	void print() const;
	Representation* convertToAdjacency();
	Representation* convertToIncidence();
	Representation* convertToAdjacencyList();
	virtual void saveToDraw() const;
	void saveType() const;
protected:
	std::vector<std::vector<int>> m_list;

};

