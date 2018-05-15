#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <stack>
#include <map>
#include <utility>
#include <memory>
#include <cmath>
#include <ctime>
#include "QDebug"
#include "boost/random.hpp"

class Vertex
{
public:
    friend class Graph;
    Vertex():m_visited(false){}
    std::vector<int> &getNeigh(){return m_neigh;}
    std::map<int,int>& getWeights(){return m_wagesTo;}
    std::vector<std::vector<int>>& getPaths(){return m_shortestPaths;}
private:
    void generatePaths(int numV);

    std::vector<int> m_neigh;
    std::map<int,int> m_wagesTo;
    std::map<int,int> m_prevNeigh;
    std::vector<std::vector<int>> m_shortestPaths;



    bool m_visited;
    int m_index;
};

#endif // VERTEX_H
