#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <queue>  
#include "../include/Graph.h" 
#include "../include/Session.h"

class Session;

class Tree{
public:
    Tree(int rootLabel);
    virtual ~Tree();
    void clear();
    virtual Tree *clone() const=0;
    void copy(const int &node, const std::vector<Tree*> children);
    Tree(const Tree& tree);
    void addChild(const Tree& child);
    void addChild(Tree* child);
    Tree(Tree&& other);
    Tree& operator = (Tree&& other);
    int getNode();
    std:: vector<Tree*> getChild() const;
    Tree& operator=(const Tree &other);
    static Tree* BFS(Session& session, int nodeIndex);
    int getRootLabel();
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;
private:
    int node;
    std::vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    virtual Tree* clone() const;
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    virtual Tree* clone() const;
    MaxRankTree(int rootLabel);
    virtual int traceTree();
};

class RootTree: public Tree{
public:
    virtual Tree* clone() const;
    RootTree(int rootLabel);
    virtual int traceTree();
};

#endif
