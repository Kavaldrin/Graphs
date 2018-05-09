#include "vertex.h"
#include <QDebug>

void Vertex::setDijkstraPaths(int w)
{
    m_computedPaths.clear();

    qInfo() << "w ustawianiu";
    std::vector<int> temp_vec;
    for(int i=0;i<w;++i){
        temp_vec.clear();
        m_computedPaths.push_back(temp_vec);
        int prev = m_prevWeights[i];
        while(prev >= 0){
            temp_vec.push_back(prev);
            prev = m_prevWeights[prev];
        }
        if(!temp_vec.empty()){
            for(int j=temp_vec.size()-1; j >= 0 ;--j )
                m_computedPaths[i].push_back(temp_vec[j]);
        }
    }
    qInfo() << "po ustawianiu";

}
