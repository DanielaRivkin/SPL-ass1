#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "../include/Graph.h"
#include <queue>   

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);
    ~Session();
    Session(const Session& session); // Copy constractor
    Session(Session&& session); // Move constractor
    Session& operator=(const Session& session); // Copy assignment constractor
    Session& operator=(Session&& session); // Move assignment constractor
    std::vector<Agent*> copy() const;
    void stealFrom(Session& session);    
    void simulate();
    void addAgent(const Agent& agent);
    Graph getGraph() const;
    void setGraph(const Graph& graph);   
    void enqueueInfected(int);
    int dequeueInfected();
    void setTreeType(std::string type);
    TreeType getTreeType() const;
    int getCycle() const;
    void clear();
    void report();
    void pop();
    bool isQueEmp();

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    std::vector<int> infque;
    std::vector<int> infOrder;
    int cycle;
};

#endif
