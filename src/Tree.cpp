#include "../include/Tree.h"
#include <queue>

using namespace std;

//constructor
Tree::Tree(int rootLabel) : node(rootLabel), children(std::vector<Tree *>()) {}


//destructor
Tree::~Tree() {
    for (auto tree:children) {
        if (tree) delete tree;
        tree = nullptr;
    }
    children.clear();
}

//copy constructer
Tree::Tree(const Tree &tree) : node(tree.node), children() {
    copy(tree.node, tree.children);
}

//copy assignment operator
Tree &Tree::operator=(const Tree &other) {
    if (this != &other) {
        for (auto tree:children) {
            if (tree) delete tree;
            tree = nullptr;
        }
        children.clear();
        copy(other.node, other.children);
    }
    return *this;
}

//help method for copy constructor
void Tree::copy(const int &node, const std::vector<Tree *> children) {
    this->node = node;
    for (unsigned i = 0; i < children.size(); i++) {
        this->children.push_back(children.at(i)->clone());
    }
}

//move constructor
Tree::Tree(Tree &&other) : node(other.node), children(other.children) {
    for (int i = 0; i < (signed) getChild().size(); i++) {
        other.children.at(i) = nullptr;
    }
    other.node = 0;
}

//move assignment operator
Tree &Tree::operator=(Tree &&other) {
    if (this != &other) {
        for (auto tree:children) {
            if (tree) delete tree;
            tree = nullptr;
        }
        children.clear();
        this->node = node;
        children = std::move(other.children);
        other.node = 0;
    }
    return *this;
}


void Tree::addChild(const Tree &child) {
    Tree *chil = child.clone();
    children.push_back((chil));
}

Tree *Tree::createTree(const Session &session, int rootLabel) {
    if (session.getTreeType() == Cycle) {
        return new CycleTree(rootLabel, session.getCycle());
    } else if (session.getTreeType() == MaxRank) {
        return new MaxRankTree(rootLabel);
    } else {
        return new RootTree(rootLabel);
    }
}

CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel), currCycle(currCycle) {
}

int CycleTree::traceTree() {
    Tree *temp = this; //requires copy constructor
    for (int i = 0; i < currCycle; i++) {
        if (temp->getChild().empty()) {
            break;
        }
        temp = temp->getChild()[0];
    }
    return temp->getNode();
}

MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) {
}

int MaxRankTree::traceTree() {
    std::queue<Tree *> que;
    que.push(this);
    int maxRank = getNode();
    int maxSize = getChild().size();
    while (!que.empty()) {
        Tree *popped = que.front();
        que.pop();
        for (auto &child: popped->getChild()) {
            if (popped->getChild().size() > (unsigned) maxSize) {
                maxRank = popped->getNode();
                maxSize = popped->getChild().size();
            }
            que.push(child);
        }
    }
    return maxRank;
}


RootTree::RootTree(int rootLabel) : Tree(rootLabel) {
}

int Tree::getNode() {
    return node;
}

std::vector<Tree *> Tree::getChild() const {
    return children;
}

Tree *Tree::BFS(Session &session, int nodeIndex) {
    Tree *output = Tree::createTree(session, nodeIndex);
    std::vector<bool> visited = vector<bool>((signed) session.getGraph().size(),
                                             false); // init false value node-visited vector
    visited.at(nodeIndex) = true;
    std::queue<Tree *> q;
    q.push(output);
    Tree *temp;
    while (!q.empty()) {
        temp = q.front();
        q.pop();
        for (int i = 0; i < (signed) session.getGraph().size(); i++) {
            if (!visited.at(i) && session.getGraph().getEdges().at(temp->getNode()).at(i) == 1) {
                visited.at(i) = true;
                Tree *child = Tree::createTree(session, i);
                q.push(child);
                temp->addChild(child);
            }
        }
    }
    return output;
}

void Tree::addChild(Tree *child) {
    children.push_back(child);
}

int RootTree::traceTree() {
    return getNode();
}

Tree *RootTree::clone() const {
    return new RootTree(*this);
}

Tree *MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}

Tree *CycleTree::clone() const {
    return new CycleTree(*this);
}

int Tree::getRootLabel() {
    return node;
}