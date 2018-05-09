#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include "vertex.h"
#include <ctime>
#include <QDebug>
#include <utility>
#include <map>
#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>


class Graph
{
public:
    Graph():m_matrixRepresentation(nullptr),m_distanceMatrix(nullptr),allDijskra(false){}
    Graph(int v);
    ~Graph();
    void generateConnected();
    std::vector<std::pair<int,int>> &getPairs(){return m_uniqConnections;}
    std::map<std::pair<int,int>,int> &getWeights(){return m_weights;}
    std::vector<Vertex> &getVertexes(){return m_vertexes;}
    std::vector<std::pair<int,int>> &getTreeConnections(){return m_treeConnections;}
    int getVertexNum(){return m_numVertex;}

    void Dijaskra(int s);
    void ComputeDistanceMatrix();
    int computeCentreVertex();
    int computeMinMax();
    void generateTreePrim();
    void generateTreeKruskal();

private:
    void initDijaskra(int s);
    int lowestFromNotReady(std::vector<int> &vec, std::map<int,int> &weights);
    void prepareUniqPair();
    void setWeights();
    void debugPrint() const;
    void convertMatrix();
    bool areConnected(int *connections, int w,int v);
    bool notIn(std::vector<int> &vertex, int v);
    void relaxDijaskra(int v,int u,int s);
    void setOrgWeights();
    bool canConnect(std::vector<std::vector<int>> &groups,std::pair<int,int> &p);
    std::pair<int,int> findMinToNotVisited(std::vector<int> &visited);
    std::vector<Vertex> m_vertexes;
    std::vector<std::pair<int,int>> m_uniqConnections;
    std::vector<std::pair<int,int>> m_treeConnections;
    std::map<std::pair<int,int>,int> m_weights;
    int **m_distanceMatrix;
    int** m_matrixRepresentation;
    int m_numVertex;
    bool allDijskra;

};


bool myCompare(std::pair<std::pair<int,int>,int> &p1,std::pair<std::pair<int,int>,int> &p2);

inline bool Graph::areConnected(int *connections, int w,int v)
{
    for(int i=0;i<w;++i){
        if(connections[i] == v)
            return true;
    }
    return false;
}






#endif // GRAPH_H
