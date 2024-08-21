#include "../include/Session.h"
#include <fstream>
#include <iostream>
#include "../include/json.hpp"
#include "../include/Agent.h"
#include "../include/Graph.h"

using namespace std;
using json = nlohmann::json;

Session::Session(const std::string& path):g(vector<vector<int>>()),
                                          treeType(Root), // Will be override 
                                          agents(vector<Agent*>()),
                                          infque(vector<int>()),
                                          infOrder(vector<int>()),
                                          cycle(0)
                                          {
    // Reading the json file into a buffer
    ifstream infile(path);
    json j;
    string agent;
    // Open json
    infile >> j;
    infile.close();
    // Parse json
    setGraph(Graph(j["graph"]));
    setTreeType(j["tree"]);
    for(auto& agent: j["agents"]){
        if (agent[0] == "V") {
            g.occupNode(agent[1]);
            agents.push_back(new Virus(agent[1])); // agents on the ***heap***
            }
        else {
            agents.push_back(new ContactTracer()); // agents on the ***heap***
        }
    }
    j.clear();
    
}
// Destructor
Session::~Session(){
    clear();
}

// Copy constractor
Session::Session(const Session& session):g(session.g),
                                        treeType(session.treeType),
                                        agents(vector<Agent*>()),
                                        infque(session.infque),
                                        infOrder(session.infOrder),
                                        cycle(session.cycle)
{
    session.copy();
    }

// Move constractor
Session::Session(Session&& session):g(session.g),
                                        treeType(session.treeType),
                                        agents(vector<Agent*>()),
                                        infque(session.infque),
                                        infOrder(session.infOrder),
                                        cycle(session.cycle)
{
    this->stealFrom(session);
}

std::vector<Agent*> Session::copy() const {// Copy agents vector
    std::vector<Agent*> newAgents;
    for (Agent* agent:agents){
        newAgents.push_back(agent);
    }
    return newAgents;
}

void Session::stealFrom(Session& session) {// Steal agents vector and set ptrs to null
    this->agents = session.agents;
    session.agents.clear();
}

// Copy assignment operator
Session& Session::operator=(const Session& session){
    if (this != &session){ 
        this->clear();
        g = session.g;
        treeType = session.treeType;
        infque = session.infque;
        infOrder = session.infOrder;
        cycle = session.cycle;
        agents = session.copy();
    }
    return *this;
}

// Move assignment constractor
Session& Session::operator=(Session&& session){
    if (this != &session){ 
        this->clear();
        g = session.g;
        treeType = session.treeType;
        infque = session.infque;
        infOrder = session.infOrder;
        cycle = session.cycle;
        stealFrom(session);
    }
    return *this;
}

void Session::clear(){
    // delete agents
    for (Agent* agent: agents){
        if (agent){
            delete agent;
            agent = nullptr;
        }
    }
    agents.clear();
    infque.clear();
}

void Session::simulate(){
    while (g.infactEnd()){
        std::vector<Agent*> currAgents = this->copy(); // runs a copy of agents vector (so cycles can be distinguish)
        for (Agent* agent: currAgents){
            agent->act(*this);
        }
        currAgents.clear();
        cycle++;
    }
    report();
}

void Session::addAgent(const Agent& agent){
    Agent* clone = agent.clone();
    agents.push_back(clone);
}
void Session::setGraph(const Graph& graph){
    g = graph;
}

Graph Session::getGraph() const{
    return g;
}
    
void Session::enqueueInfected(int infnode){
    infque.push_back(infnode);
    infOrder.push_back(infnode);
}
int Session::dequeueInfected(){
    int front = infque.front();
    pop();
    return front;
}

int Session::getCycle() const{
    return cycle;
}

TreeType Session::getTreeType() const{
    return treeType;
}

void Session::setTreeType(std::string type){
    if (type == "M") treeType = MaxRank;
    else if (type == "R") treeType = Root;
    else treeType = Cycle;
 }

void Session::report(){ // Creates output json
    json j;
    j["infected"] = infOrder;
    j["graph"] = g.getEdges();
    ofstream outfile("output.json");
    outfile << j;
    outfile.close();
}

void Session::pop(){
    if ((signed)infque.size() != 0) infque.erase(infque.begin());
}

bool Session::isQueEmp(){
    if ((signed)infque.size() == 0) return true;
    return false;
}