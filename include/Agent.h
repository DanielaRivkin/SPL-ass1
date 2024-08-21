#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include "../include/Session.h"

class Agent{
public:
    Agent();
    virtual void act(Session& session)=0;
    virtual Agent* clone() const=0;
    virtual ~Agent()=default;
};

class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual ContactTracer* clone() const;
    virtual void act(Session& session);
    virtual ~ContactTracer();
};

class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual Virus* clone() const;
    virtual void act(Session& session);
    virtual ~Virus();
private:
    const int nodeInd;
};

#endif