#include "vertex.h"


void Vertex::generatePaths(int numV)
{

    std::vector<int> temp_v;
    int prev;

    for(int i=0;i<numV;++i){
        m_shortestPaths.push_back(temp_v);
        prev = m_prevNeigh[i];
        while(prev != -1){
            m_shortestPaths[i].push_back(prev);
            prev = m_prevNeigh[prev];
        }
    }
}
