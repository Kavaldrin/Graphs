#include "graph.h"


Graph::Graph(int v):m_numVertex(v)
{
    m_matrixRepresentation = new int*[v];
    for(int i=0;i<v;++i){
        m_matrixRepresentation[i] = new int[v];
    }

    for(int i=0;i<v;++i){
        for(int j=0;j<v;++j){
            m_matrixRepresentation[i][j] = 0;
        }
    }
}
Graph::~Graph()
{
    if(m_matrixRepresentation){
        for(int i=0;i<m_numVertex;++i){
            delete [] m_matrixRepresentation[i];
        }
        delete [] m_matrixRepresentation;
    }

    if(m_distanceMatrix){
        for(int i=0;i<m_numVertex;++i){
            delete [] m_distanceMatrix[i];
        }
        delete [] m_distanceMatrix;
    }
}

void Graph::generateConnected()
{
    if(!m_vertexes.empty()){
        m_vertexes.clear();
        for(int i=0;i<m_numVertex;++i){
            delete [] m_matrixRepresentation[i];
        }
        delete [] m_matrixRepresentation;
        m_matrixRepresentation = nullptr;
        m_uniqConnections.clear();
        m_weights.clear();
    }

    if(m_distanceMatrix){
        for(int i=0;i<m_numVertex;++i){
            delete[] m_distanceMatrix[i];
        }
        delete[] m_distanceMatrix;
        m_distanceMatrix = nullptr;

    }

    allDijskra = false;

    using namespace std;

    const int max_ver = 15;
    const int min_ver = 2;
    int numVer = (rand() %  (max_ver - min_ver)) + min_ver;

    ///////// inicjalization of graph memory and variables
    m_matrixRepresentation = new int*[numVer];
    for(int i=0;i<numVer;++i)
        m_matrixRepresentation[i] = new int[numVer];

    for(int i=0;i<numVer;++i){
        for(int j=0;j<numVer;++j)
            m_matrixRepresentation[i][j] = 0;
    }
    m_numVertex = numVer;
    /////////////////////////////////


    const int max_connections = numVer * (numVer-1) / 2;
    const int min_connections = numVer-1;
    int numCon;
    if(max_connections != min_connections)
        numCon = (rand() % (max_connections - min_connections)) + min_connections;
    else
        numCon = 1;



    vector<int> notConnected;
    for(int i=0;i<numVer;++i){
        notConnected.push_back(i);
    }
    vector<int> connected;
    //doing first connection
    int indexV1,indexV2;
    indexV1 = rand() % notConnected.size();
    indexV2 = rand() % notConnected.size();
    while(indexV1 == indexV2)
        indexV2 = rand() % notConnected.size();

    //bug
    if(indexV1 > indexV2){
        notConnected.erase(notConnected.begin()+indexV1);
        notConnected.erase(notConnected.begin()+indexV2);
    }
    else{
        notConnected.erase(notConnected.begin()+indexV2);
        notConnected.erase(notConnected.begin()+indexV1);
    }
    //

    connected.push_back(indexV1);
    connected.push_back(indexV2);
    m_matrixRepresentation[indexV1][indexV2] = 1;
    m_matrixRepresentation[indexV2][indexV1] = 1;

    --numCon;
    //first connecting not connected vertexes to make connectivity
    //double condition to be sure of validity
    while(!notConnected.empty()){
        indexV1  = rand() % notConnected.size();
        indexV2 = rand() % connected.size();
        connected.push_back(notConnected[indexV1]);
        m_matrixRepresentation[notConnected[indexV1]][connected[indexV2]] = 1;
        m_matrixRepresentation[connected[indexV2]][notConnected[indexV1]] = 1;

        notConnected.erase(notConnected.begin() + indexV1);
        --numCon;

    }
    //now we have connectivity so lets connect other random
    while(numCon > 0){
        indexV1 = rand() % numVer;
        indexV2 = rand() % numVer;
        if(indexV1 != indexV2 && !areConnected(m_matrixRepresentation[indexV1],numVer,indexV2)){
              m_matrixRepresentation[indexV1][indexV2] = 1;
              m_matrixRepresentation[indexV2][indexV1] = 1;
              --numCon;
        }
    }
    convertMatrix();
    prepareUniqPair();
    setWeights();
    debugPrint();
}

void Graph::convertMatrix()
{
    if(m_vertexes.empty())
        m_vertexes.clear();

    for(int i=0;i<m_numVertex;++i){
        m_vertexes.push_back(Vertex());
        m_vertexes[i].m_index = i;
        for(int j=0;j<m_numVertex;++j){
            if(m_matrixRepresentation[i][j]){
                m_vertexes[i].m_connections.push_back(j);
            }
        }
        m_vertexes[i].m_numCon = m_vertexes[i].m_connections.size();
    }
}

void Graph::debugPrint() const
{
    qInfo() << "Matrix: \n";
    for(int i=0;i<m_numVertex;++i){
        QDebug deb = qDebug();
        for(int j=0;j<m_numVertex;++j){
            deb << m_matrixRepresentation[i][j];
        }
    }
    for(unsigned i=0;i<m_vertexes.size();++i){
        QDebug deb = qDebug();
        deb << i <<": ";
        for(auto & neigh : m_vertexes[i].m_connections){
            deb << neigh;
        }
    }
}

void Graph::setWeights()
{
    for(auto &pr : m_uniqConnections){
        m_weights[pr] = (rand() % 10) +1;
    }
}

void Graph::prepareUniqPair()
{
    for(int i=0;i<m_numVertex;++i){
        for(int j=i;j<m_numVertex;++j){
            if(m_matrixRepresentation[i][j]){
                m_uniqConnections.push_back(std::make_pair(i,j));
            }
        }
    }
}

void Graph::Dijaskra(int s)
{
    initDijaskra(s);
    std::vector<int> ready;
    std::vector<int> notReady;
    for(int i=0;i<m_numVertex;++i){
        notReady.push_back(i);
    }

    int u;
    while(ready.size() != m_numVertex){
        u = lowestFromNotReady(notReady,m_vertexes[s].m_nextWeighs);
        auto it = std::find(notReady.begin(),notReady.end(),u);
        ready.push_back(u);
        qInfo() << "przed" << ready.size();
        notReady.erase(it);
        qInfo() << "po" << ready.size();
        for(auto &v : m_vertexes[u].m_connections){
            if(notIn(ready,v)){
                relaxDijaskra(v,u,s);
            }
        }
    }

    m_vertexes[s].setDijkstraPaths(m_numVertex);
    QDebug deb = qDebug();
    for(int i=0;i<m_numVertex;++i){
        deb << i+1 << " : "<< m_vertexes[s].m_nextWeighs[i] << "\n";
    }
    for(int i=0;i<m_numVertex;++i){
        deb << i+1 << " : ";
        for(auto &n : m_vertexes[s].m_computedPaths[i])
            deb << n+1;
        deb <<"\n";
    }


}

void Graph::relaxDijaskra(int v,int u,int s)
{
    std::pair<int,int> pr = std::make_pair(v,u);

    try{
        m_weights.at(pr);
    }
    catch(...){
        pr = std::make_pair(u,v);
    }

    if(m_vertexes[s].m_nextWeighs[v] > m_vertexes[s].m_nextWeighs[u] + m_weights[pr]){
        m_vertexes[s].m_nextWeighs[v] = m_vertexes[s].m_nextWeighs[u] + m_weights[pr];
        m_vertexes[s].m_prevWeights[v] = u;
    }
}


bool Graph::notIn(std::vector<int> &vertex, int v)
{
    for(auto &n : vertex){
        if(n == v)
            return false;
    }
    return true;
}

void Graph::initDijaskra(int s)
{
    if(!m_vertexes[s].m_nextWeighs.empty()){
        m_vertexes[s].m_nextWeighs.clear();
        m_vertexes[s].m_prevWeights.clear();
    }


    for(int i=0;i<m_numVertex;++i){
       m_vertexes[s].m_nextWeighs[i] = INT_MAX;
       m_vertexes[s].m_prevWeights[i] = -1; // NILL
    }
    m_vertexes[s].m_nextWeighs[s] = 0;
}
int Graph::lowestFromNotReady(std::vector<int> &vec,std::map<int,int> &weights)
{
    int min = vec.front();
    for(auto &index : vec){
        if(weights[min] > weights[index]){
            min = index;
        }
    }
    return min;
}

void Graph::ComputeDistanceMatrix()
{
    if(!allDijskra){
        for(int i=0;i<m_numVertex;++i)
            Dijaskra(i);
        allDijskra = true;
    }

    m_distanceMatrix = new int*[m_numVertex];
    for(int i=0;i<m_numVertex;++i){
        m_distanceMatrix[i] = new int[m_numVertex];
    }

    for(int i=0;i<m_numVertex;++i){
        for(int j=0;j<m_numVertex;++j){
            m_distanceMatrix[i][j] = m_vertexes[i].m_nextWeighs[j];
        }
    }
    allDijskra = true;

}

int Graph::computeCentreVertex()
{
    int minIndex = 0;
    int minSum = INT_MAX;
    int temp_sum = 0;

    if(!allDijskra){
        for(int i=0;i<m_numVertex;++i)
            Dijaskra(i);
        allDijskra = true;
    }

    for(int i=0;i<m_numVertex;++i){
        temp_sum = 0;
        for(int j=0;j<m_numVertex;++j)
            temp_sum += m_vertexes[i].m_nextWeighs[j];
        if(minSum > temp_sum){
            minIndex = i;
            minSum = temp_sum;
        }
    }

    return minIndex;
}

int Graph::computeMinMax()
{
    std::vector<int> maxes;
    int temp_max;
    for(int i=0;i<m_numVertex;++i){
        temp_max = m_vertexes[i].m_nextWeighs[0];
        for(int j=1;j<m_numVertex;++j){
            if(m_vertexes[i].m_nextWeighs[j] > temp_max){
                temp_max = m_vertexes[i].m_nextWeighs[j];
            }
        }
        maxes.push_back(temp_max);
    }

   int minIndex = 0;
   for(int i=1;i<m_numVertex;++i){
       if(maxes[minIndex] > maxes[i] )
           minIndex = i;
   }
   return minIndex;

}

void Graph::setOrgWeights()
{
    for(auto pr : m_uniqConnections){
        try{
            m_weights.at(pr);
        }
        catch(...){
            pr = std::make_pair(pr.second,pr.first);
        }

        m_vertexes[pr.first].m_orgWeights[pr.second] = m_weights[pr];
        m_vertexes[pr.second].m_orgWeights[pr.first] = m_weights[pr];
    }

    /*
    for(auto &vertex : m_vertexes){
        QDebug deb = qDebug();
        deb << vertex.m_index;
        for(auto &n : vertex.m_connections){
            deb << n << ":" << vertex.m_orgWeights[n] << " ";
        }
    }
    */
}

std::pair<int,int> Graph::findMinToNotVisited(std::vector<int> &visited)
{
    int indexNot,indexVisited;
    int minWeight = INT_MAX;

    for(auto &v : visited){
        for(auto &n : m_vertexes[v].m_connections){
            if((minWeight > m_vertexes[v].m_orgWeights[n]) && (notIn(visited,n))){
                qDebug() << "N " << n;
                indexNot = n;
                indexVisited = v;
                minWeight = m_vertexes[v].m_orgWeights[n];
                qDebug() << minWeight;
            }
        }

    }
    qInfo() << indexNot << indexVisited;
    return std::make_pair(indexNot,indexVisited);
}

void Graph::generateTreePrim()
{



    m_treeConnections.clear();
    std::vector<int> visited;
    setOrgWeights();

    int indexV1;

    indexV1 = rand() % m_numVertex;
    visited.push_back(indexV1);



    while(visited.size() != m_numVertex){
        std::pair<int,int> pr = findMinToNotVisited(visited);
        visited.push_back(pr.first);

        try{
            m_weights.at(pr);
        }
        catch(...){
            pr = std::make_pair(pr.second,pr.first);
        }


        m_treeConnections.push_back(pr);


    }

    for(auto & pr : m_treeConnections){
        qInfo() << pr.first << pr.second;
    }
}

void Graph::generateTreeKruskal()
{
    m_treeConnections.clear();

    std::vector<std::pair<std::pair<int,int>,int>> pairs;
    for(auto &pr : m_uniqConnections){
        try{
            m_weights.at(pr);
        }
        catch(...){
            pr = std::make_pair(pr.second,pr.first);
        }
        pairs.push_back(std::make_pair(pr,m_weights[pr]));
    }

    std::sort(pairs.begin(),pairs.end(),myCompare);

    /*
    QDebug deb = qDebug();
    for(auto &pr : pairs){
        deb << pr.first.first << pr.first.second << pr.second;
    }
    */

    std::vector<std::vector<int>> groups;
    std::vector<int> temp;

    m_treeConnections.push_back(pairs.back().first);
    pairs.pop_back();
    temp.push_back(m_treeConnections[0].first);
    temp.push_back(m_treeConnections[0].second);


    groups.push_back(temp);

    while(!pairs.empty() && m_treeConnections.size() < m_numVertex-1){
        std::pair<int,int> p = pairs.back().first;
        pairs.pop_back();
        if(canConnect(groups,p)){
            m_treeConnections.push_back(p);
        }
    }


}
bool Graph::canConnect(std::vector<std::vector<int>> &groups,std::pair<int,int> &p)
{
    int indexF = -1, indexS = -1;
    for(int i=0;i<groups.size(); ++i){
        for(auto &n : groups[i]){
            if(n == p.first)
                indexF = i;
            else if(n == p.second)
                indexS = i;
        }
    }

     // jak sa w tej samej tzn zrobia cykl
    if(indexF == indexS && indexF != -1)
        return false;

    // jak nie ma ich w ogole -> tworza nowa grupe
    if(indexF == indexS && indexF == -1){
        if(!groups.back().empty()){
            std::vector<int> temp;
            groups.push_back(temp);
        }
        groups.back().push_back(p.second);
        groups.back().push_back(p.first);
        return true;
    }

    //jak jedno jest a druegio nie ma
    if(indexF != -1 && indexS == -1){
        groups[indexF].push_back(p.second);
        return true;
    }

    //jw
    if(indexF == -1 && indexS != -1){
        groups[indexS].push_back(p.first);
        return true;
    }

    //mergowanie 2 osobnych grup
    if(indexF >= 0 && indexS >= 0 && indexS != indexF){
        for(auto &n : groups[indexS]){
            groups[indexF].push_back(n);
        }
        groups[indexF].push_back(p.second);
        groups.erase(groups.begin() + indexS);
        return true;
    }
    //blad
    qDebug() << "blad" << indexF << indexS;
    return false;
}

bool myCompare(std::pair<std::pair<int,int>,int> &p1,std::pair<std::pair<int,int>,int> &p2)
{
    return p2.second < p1.second;
}
