#ifndef AGENTLEMMINGTEST_H_
#define AGENTLEMMINGTEST_H_

#include "Agent.h"
#include "NeuronalNetwork/NeuronalNetworkMonocouche.h"

class AgentLemmingTest : public Agent
{
private:
	NeuronalNetworkMonocouche ArtificialInteligence;

public:
	AgentLemmingTest(Body* body);

	virtual void live();

};

#endif
