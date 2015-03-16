#include "Agent.h"


Agent::Agent(Body* body)
{
	this->body = body;
}

void Agent::Life()
{
	std::vector<PhysicalObject*>* listOf = this->body->GetPerception();

	this->body->SetIntention(rand()%5-2, rand()%5-2, 1);
}

Agent::~Agent(void)
{
}
