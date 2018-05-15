#include "graph.h"

void Graph::generateDirectedGraph(int numV,double numP)
{
    boost::random::mt19937 rng;
    rng.seed(time(NULL));
    boost::random::uniform_real_distribution<> range(0.,1.);

    m_representations = false;
    m_generatedCon = false;
    m_fordDone = false;
    m_johnsonDone = false;
    m_negativeCycle = true;
    lastBellman = -1;
    m_connections.clear();
    m_vertexes.clear();
    m_strongComps.clear();

    Vertex v_temp;
    for(int i=0;i<numV;++i){
        m_vertexes.push_back(v_temp);
        m_vertexes[i].m_index = i;
    }

    for(int i=0;i<numV;++i){
        for(int j=0;j<numV;++j){
            if(i != j && range(rng) < numP){
                m_vertexes[i].m_neigh.push_back(j);
                m_connections.push_back(std::make_pair(i,j));
            }
        }
    }

}

void Graph::Kosaraju()
{

    std::stack<int> s;

    for(auto &vertex : m_vertexes){
        if(!vertex.m_visited){
            DFS_visit(vertex.m_index,s);
        }
    }

    Graph copy = TranslateGraph();
    std::vector<int> temp_vec;


    int curr_v;
    while(!s.empty()){
        curr_v = s.top();
        if(!copy.m_vertexes[curr_v].m_visited){
            copy.m_vertexes[curr_v].m_visited = true;
            m_strongComps.push_back(temp_vec);
            m_strongComps.back().push_back(curr_v);
            ComponentsR(curr_v,copy);
        }
            s.pop();

    }


}


void Graph::DFS_visit(int index, std::stack<int> &s)
{
    m_vertexes[index].m_visited=true;
    for(auto &n : m_vertexes[index].m_neigh){
        if(!m_vertexes[n].m_visited){
            DFS_visit(n,s);
        }
    }
    s.push(index);
}

Graph Graph::TranslateGraph()
{
    Graph graf;
    Vertex v_temp;
    for(int i=0;i<m_vertexes.size();++i) graf.m_vertexes.push_back(v_temp);


    for(auto &vertex : m_vertexes){
        for(auto &n: vertex.m_neigh){
            graf.m_vertexes[n].m_neigh.push_back(vertex.m_index);
        }
    }
    return graf;
}
void Graph::ComponentsR(int index,Graph &copy)
{
    for(auto &n : copy.m_vertexes[index].m_neigh){
        if(!copy.m_vertexes[n].m_visited){
            copy.m_vertexes[n].m_visited = true;
            m_strongComps.back().push_back(n);
            ComponentsR(n,copy);
        }
    }

}

void Graph::generateStrongConnected(int numV, double numP)
{

    m_generatedCon = false;
    boost::random::mt19937 rng;
    rng.seed(time(NULL));
    boost::random::uniform_int_distribution<> range(-5,10);


    if(numP < 0.1)
        numP+=0.1;
    int attempts = 10;

    while(!m_generatedCon && attempts){
        generateDirectedGraph(numV,numP);
        Kosaraju();
        --attempts;

        if(m_strongComps.size() == 1)
            m_generatedCon = true;

        if(!attempts){
            numP+=0.1;
            attempts = 10;
        }
    }

    int value;
    for(auto &pr : m_connections){
        value = range(rng);
        if(value<0)
            value = range(rng);
        m_wages[pr] = value;
    }
}

bool Graph::BellmanFord(int source)
{

    m_vertexes[source].m_prevNeigh.clear();
    m_vertexes[source].m_wagesTo.clear();
    m_vertexes[source].m_shortestPaths.clear();

    BellmanFordInit(source);

    /*
    for(auto &pr : m_connections){
        qInfo() << pr.first << pr.second << m_wages[pr];
    }
    */


    for(int i=0;i<m_vertexes.size()-1;++i){
        for(auto &pr : m_connections){
            BellmanFordRelax(pr.first,pr.second,source);
        }
    }


    for(auto &pr : m_connections){
        if(m_vertexes[source].m_wagesTo[pr.second] > m_vertexes[source].m_wagesTo[pr.first] + m_wages[pr]){
            lastBellman = -1;
            m_fordDone = true;
            m_negativeCycle = true;
            return false;
        }
    }

    m_fordDone = true;
    lastBellman = source;
    m_vertexes[source].generatePaths(m_vertexes.size());


    /*
    for(int i=0;i<m_vertexes.size();++i){
        qInfo() << m_vertexes[source].m_wagesTo.at(i);
    }
    */

    m_negativeCycle = false;
    return true;

}

void Graph::BellmanFordInit(int source)
{
    for(int i=0;i<m_vertexes.size();++i){
        m_vertexes[source].m_wagesTo[i] = 100000;
        m_vertexes[source].m_prevNeigh[i] = -1; //null
    }
    m_vertexes[source].m_wagesTo[source] = 0;
}

void Graph::BellmanFordRelax(int u, int v,int source)
{
    if(m_vertexes[source].m_wagesTo[v] > m_vertexes[source].m_wagesTo[u] + m_wages.at(std::make_pair(u,v))){

        m_vertexes[source].m_wagesTo[v] = m_vertexes[source].m_wagesTo[u] + m_wages.at(std::make_pair(u,v));
        m_vertexes[source].m_prevNeigh[v] = u;
    }

}
void Graph::Dijaskra(int s)
{
    BellmanFordInit(s);
    std::vector<int> ready;
    std::vector<int> notReady;
    for(int i=0;i<m_vertexes.size();++i){
        notReady.push_back(i);
    }

    int u;
    while(ready.size() != m_vertexes.size()){
        u = lowestFromNotReady(notReady,m_vertexes[s].m_wagesTo);
        auto it = std::find(notReady.begin(),notReady.end(),u);
        ready.push_back(u);
        notReady.erase(it);
        for(auto &v : m_vertexes[u].m_neigh){
            if(In(notReady,v)){
                BellmanFordRelax(u,v,s);
            }
        }
    }
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
bool Graph::In(std::vector<int> &vec, int v)
{
    for(auto &n : vec){
        if(v == n)
            return true;
    }
    return false;
}


void Graph::Johnson()
{
    m_johnsonMatrix.clear();


    std::vector<int> temp_v;
    for(int i=0;i<m_vertexes.size();++i){
        m_johnsonMatrix.push_back(temp_v);
        for(int j=0;j<m_vertexes.size();++j)
            m_johnsonMatrix[i].push_back(0);
     }


    std::map<int,int> h;

    Graph graphPrim = addS();
    if(!graphPrim.BellmanFord(m_vertexes.size())){ //dodatkowy czyli ostatni
        m_johnsonDone = true;
        m_negativeCycle = true;
        return;
     }
    for(auto &vertex : graphPrim.m_vertexes){
        h[vertex.m_index] = graphPrim.m_vertexes[m_vertexes.size()].m_wagesTo[vertex.m_index];
    }

    for(auto &pr : graphPrim.m_connections){
        graphPrim.m_wages[pr] = m_wages[pr] + h[pr.first] - h[pr.second];
    }

    std::map<std::pair<int,int>,int> copy_map = m_wages;
    m_wages = graphPrim.m_wages;

    for(auto &vertex : m_vertexes){
        Dijaskra(vertex.m_index);
        for(auto &vertex2 : m_vertexes){
            m_johnsonMatrix[vertex.m_index][vertex2.m_index] = m_vertexes[vertex.m_index].m_wagesTo[vertex2.m_index] - h[vertex.m_index] + h[vertex2.m_index];
        }

    }

    m_wages = copy_map;

    /*
    for(auto &vec : m_johnsonMatrix){
        QDebug deb = qDebug();
        for(auto &num : vec){
            deb << num;
        }
    }
    */
    m_johnsonDone = true;
    m_negativeCycle = false;



}
Graph Graph::addS()
{
    Graph g(*this);
    for(int i=0;i<m_vertexes.size();++i){
        g.m_connections.push_back(std::make_pair(m_vertexes.size(),i));
        g.m_wages[std::make_pair(m_vertexes.size(),i)] = 0;
    }
    g.m_vertexes.push_back(Vertex());
    g.m_vertexes[m_vertexes.size()].m_index = m_vertexes.size();
    for(int i=0;i<m_vertexes.size();++i){
        g.m_vertexes[m_vertexes.size()].m_neigh.push_back(i);
    }
   return g;
}
