# include "../include/Agent.h"
# include "../include/Session.h"
# include "../include/Tree.h"
# include <iostream>

Agent::Agent(){}

ContactTracer::ContactTracer(){}

void ContactTracer::act(Session& session){
    if (!session.isQueEmp()){
        int toInf = session.dequeueInfected();
        Tree* treePtr = Tree::BFS(session,toInf);
        int toCute = treePtr->traceTree();
        delete treePtr;
        treePtr = nullptr;
        Graph g = session.getGraph();
        g.cutNode(toCute);
        session.setGraph(g);
    }
}

ContactTracer* ContactTracer::clone()const{
    return new ContactTracer();
}

ContactTracer::~ContactTracer(){}

Virus::Virus(int nodeInd):nodeInd(nodeInd){}

Virus::~Virus(){}

Virus* Virus::clone()const{
    return new Virus(nodeInd);
}

void Virus::act(Session& session){
    Graph g = session.getGraph();
    if (!g.isInfected(nodeInd)){
            g.infectNode(nodeInd);
            session.enqueueInfected(nodeInd); // Add to infaction queue
    }
    for (int i=0; i < (signed)g.getChild(nodeInd).size(); i++){
        if (g.getChild(nodeInd)[i] && !g.isOccup(i)){ // Get child with lowest index that is virus free
            session.addAgent(Virus(i));
            g.occupNode(i); // Mark as occupied node
            i = (signed)g.getChild(nodeInd).size(); // brack
        }
    }
    session.setGraph(g);
}