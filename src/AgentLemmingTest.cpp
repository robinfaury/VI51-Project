#include "AgentLemmingTest.h"

AgentLemmingTest::AgentLemmingTest(Body* body) : Agent(body)
{

}

void AgentLemmingTest::live()
{
	// If there is no linked body, do nothing
	if (!isLinked())
		return;

	this->body->setInfluence(ACTIONS::A_RIGHT);
}