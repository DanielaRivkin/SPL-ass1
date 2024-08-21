#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);
    void occupNode(int nodeInd);
    bool isOccup(int nodeInd);
    void cutNode(int nodeInd);
    std::vector<int> getChild(int nodeInd);
    bool infactEnd();
    int size();
    std::vector<std::vector<int>> getEdges();
private:
    std::vector<std::vector<int>> edges;
    std::vector<bool> infStat;
    std::vector<bool> occuStat;
    int numOfInf=0;
    int numOfOccu=0;
    };

#endif
