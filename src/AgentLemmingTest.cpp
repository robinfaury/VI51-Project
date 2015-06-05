#include "AgentLemmingTest.h"

AgentLemmingTest::AgentLemmingTest(Body* body) : Agent(body), ArtificialInteligence("res/learn.txt")
{
	ArtificialInteligence.learning();
	ArtificialInteligence.testing();
}

void AgentLemmingTest::live()
{
	// If there is no linked body, do nothing
	if (!isLinked())
		return;

	Perception* p = this->body->getPerception();

	this->body->setInfluence(ACTIONS::A_RIGHT);
}
