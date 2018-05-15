#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"


class Graph
{
public:

    Graph():lastBellman(-1),m_representations(false),m_generatedCon(false),m_fordDone(false),m_johnsonDone(false){}
    bool isGenerated(){return !m_vertexes.empty();}
    bool isStrongConnected() {return m_generatedCon;}
    bool isBellmanDone(){return m_fordDone;}
    void disableConnected() {m_generatedCon = false;}
    void disableFord(){m_fordDone = false;}
    void enableFord(){if(lastBellman != -1) m_fordDone = true;}
    int getBellmanIndex(){return lastBellman;}
    bool doneKosaraju(){return !m_strongComps.empty();}
    int getSize(){return m_vertexes.size();}
    bool doneJohnson(){return m_johnsonDone;}
    bool JohnsonFound(){return !m_negativeCycle;}
    bool BellmanFound(){return !m_negativeCycle;}



    std::vector<std::pair<int,int>>& getConnections(){return m_connections;}
    std::vector<Vertex>& getVertexes(){return m_vertexes;}
    std::vector<std::vector<int>>& getStrongComponents(){return m_strongComps;}
    std::map<std::pair<int,int>,int>& getWages(){return m_wages;}
    std::vector<std::vector<int>>& getJohnsonMatrix(){return m_johnsonMatrix;}


    void generateDirectedGraph(int numV, double numP);
    //
    void generateStrongConnected(int numV, double numP);
    //
    void Kosaraju();
    void ComponentsR(int index,Graph& copy);
    Graph TranslateGraph();
    //
    bool BellmanFord(int source);
    void BellmanFordRelax(int u,int v,int source);
    void BellmanFordInit(int source);
    //
    void Dijaskra(int s);
    int lowestFromNotReady(std::vector<int> &vec,std::map<int,int> &weights);
    bool In(std::vector<int> &vec, int v);

    //
    void Johnson();
    Graph addS();



private:
    void DFS_visit(int index,std::stack<int> &s);



    std::vector<Vertex> m_vertexes;
    std::vector<std::pair<int,int>> m_connections;
    std::vector<std::vector<int>> m_strongComps;
    std::map<std::pair<int,int>,int> m_wages;
    std::vector<std::vector<int>> m_johnsonMatrix;

    int lastBellman;
    bool m_representations;
    bool m_generatedCon;
    bool m_fordDone;
    bool m_johnsonDone;
    bool m_negativeCycle;

};

#endif // GRAPH_H
