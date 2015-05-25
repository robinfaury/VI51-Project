#include "Agent.h"


Agent::Agent(Body* body)
{
	this->body = body;
}

void Agent::live()
{
    //TODO: redo
	/*std::vector<int>* FoV = this->body->getPerception();

	if ((*FoV)[1] == PILL)
		this->body->setInfluence(0, -1, 1);
	else if ((*FoV)[3] == PILL)
		this->body->setInfluence(-1, 0, 1);
	else if ((*FoV)[4] == PILL)
		this->body->setInfluence(1, 0, 1);
	else if ((*FoV)[6] == PILL)
		this->body->setInfluence(0, 1, 1);
*/
}

Agent::~Agent(void)
{
}
