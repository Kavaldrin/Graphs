#ifndef VERTEX_H
#define VERTEX_H
#include <vector>
#include <map>

class Vertex
{
public:
    friend class Graph;
    Vertex(){}
    ~Vertex(){}
    std::vector<std::vector<int>>& getPaths() {return m_computedPaths;}
    std::map<int,int> &getWeights() {return m_nextWeighs;}

private:
    int m_index;
    int m_numCon;
    void setDijkstraPaths(int w);
    std::vector<int> m_connections;
    std::map<int,int> m_nextWeighs;
    std::map<int,int> m_prevWeights;
    std::map<int,int> m_orgWeights;
    std::vector<std::vector<int>> m_computedPaths;
};

#endif // VERTEX_H
