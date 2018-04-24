#pragma once


#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <utility>


class Representation
{
public:
	virtual ~Representation(){}
	virtual void print() const = 0;
	virtual Representation* convertToAdjacency() = 0;
	virtual Representation* convertToIncidence() = 0;
	virtual Representation* convertToAdjacencyList() = 0;
	virtual void saveType() const = 0;
	virtual void saveToDraw() const = 0;

protected:
	Representation(const unsigned size) :m_size(size) {}
	const unsigned m_size;

private:
	Representation() = delete;
};

