#include "../include/Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(std::vector<std::vector<int>> matrix):
                                            edges(matrix),
                                            infStat(vector<bool>((signed)matrix.size(), false)),
                                            occuStat(vector<bool>((signed)matrix.size(), false)){}

void Graph::infectNode(int nodeInd){
    infStat.at(nodeInd) = true;
    numOfInf++;
}
bool Graph::isInfected(int nodeInd){
    return infStat.at(nodeInd);
}

void Graph::occupNode(int nodeInd){
    occuStat.at(nodeInd) = true;
    numOfOccu++;
}
bool Graph::isOccup(int nodeInd){
    return occuStat.at(nodeInd);
}
void Graph::cutNode(int nodeInd){ // disconnect a node from it's neighbors
    for (int i=0; i<(signed)edges.size();i++){
        edges[nodeInd][i] = 0;
        edges[i][nodeInd] = 0;
    }
}
vector<int> Graph::getChild(int nodeInd){
    return edges[nodeInd];
}

int Graph::size(){
    return (signed)edges.size();
}

bool Graph::infactEnd(){
    return (numOfOccu > numOfInf);
}

std::vector<std::vector<int>> Graph::getEdges() {
    return edges;
}