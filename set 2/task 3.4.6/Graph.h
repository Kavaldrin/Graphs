#pragma once
#include <vector>
#include "Vertex.h"
#include <ctime>
#include <map>
#include <stack>
#include <algorithm>
#include <fstream>
#include "myException.h"
#include <string>

class Graph
{
public:
	Graph(){}
	~Graph(){}
	void addVertex(Vertex v) { vertexes.push_back(v);}
	void testPrint() const;
	void groupComponents();
	std::vector<int> findLargestGroup();
	bool eulerPossibility();
	std::vector<int>  generateEuler();
	std::vector<int> generateHamilton();
	void randomizeGraph(); //copied from last programm
	std::vector<Vertex> & getVertexes(){ return vertexes; }
	void saveToDraw(std::string type,std::vector<int>& path);
	void EulerMode();
	void HamiltonMode();
	bool HamiltonNext(int v,std::vector<int> &s);
	void onlyComponents();

private:
	bool areConnected(int v1, int v2);
	void disconnect(int v1, int v2);
	void connect(int v1, int v2);
	void componentsR(char group, Vertex &v);
	void resetVisited();
	bool uniq_elems(std::vector<int> &s);
	std::vector<Vertex> vertexes;
	bool m_connectivity;
};

