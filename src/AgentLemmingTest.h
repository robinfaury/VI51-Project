#ifndef AGENTLEMMINGTEST_H_
#define AGENTLEMMINGTEST_H_

#include "Agent.h"

class AgentLemmingTest : public Agent
{
private:

public:
	AgentLemmingTest(Body* body);

	virtual void live();

};

#endif
