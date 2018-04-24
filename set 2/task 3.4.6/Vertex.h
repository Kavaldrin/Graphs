#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
class Vertex
{
public:
	friend class Graph;
	Vertex():m_visited(false),m_group(0){}
	~Vertex(){}
	void addNeighbour(int n){ m_connections.push_back(n); }
	void sortNeighbours() { std::sort(m_connections.begin(), m_connections.end()); }
	void setIndex(int i) { m_index = i;}
	int getIndex() { return m_index; }
	std::vector<int>& getNeighbours() { return m_connections; }
	
private:
	int m_index;
	bool m_visited; //flag for algorithm
	int m_group; //flag for algorithm
	std::vector<int> m_connections;
};

