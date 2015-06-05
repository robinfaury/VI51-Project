#ifndef AGENTLEMMINGDUMMY_H_
#define AGENTLEMMINGDUMMY_H_

#include "Agent.h"

class AgentLemmingDummy : public Agent
{
protected:
    bool dumbizator;

public:
	AgentLemmingDummy(Body* body = NULL);

    /** The function containing the decision taking. This function is called once per frame.
    *
    */
	virtual void live();
};

#endif
