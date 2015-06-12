#include "AgentQLearning.h"


AgentQLearning::AgentQLearning()
{
}

AgentQLearning::AgentQLearning(Problem* problem)
{
	this->m_problem = problem;
}

AgentQLearning::~AgentQLearning()
{
}

void AgentQLearning::live()
{
	if (!this->isLinked())
	{
		return;
	}

	// Getting perception and corresponding state
	Perception* perception = this->body->getPerception();
	ProblemState* state = m_problem->convertPerceptionToState(perception, false);
	std::string action;

	// If state is null : no state exists from this perception. Then go random
	if (state == NULL)
	{
		std::cout << "PROBLEM : AgentQLearning::Live : unknown state. Choosing random action..." << std::endl;
		std::vector<std::string> actions;
		actions.push_back("left");
		actions.push_back("right");
		actions.push_back("down");
		action = ProblemStore::getOneOf(&actions);
		if (AGENTQLEARNING_DEBUG)
			std::cout << "Going random" << std::endl;
	}
	else
	{
		action = m_problem->getProblemStore()->getBestAction(state);
		if (AGENTQLEARNING_DEBUG)
			std::cout << "Finding best action : " << action  << std::endl;
	}
	ACTIONS influence;
	if (action == "down")
	{
		influence = ACTIONS::A_DOWN;
		if (AGENTQLEARNING_DEBUG)
			std::cout << "Agent going down" << std::endl;
	}
	else if (action == "left")
	{
		influence = ACTIONS::A_LEFT;
		if (AGENTQLEARNING_DEBUG)
			std::cout << "Agent going left" << std::endl;
	}
	else if (action == "right")
	{
		influence = ACTIONS::A_RIGHT;
		if (AGENTQLEARNING_DEBUG)
			std::cout << "Agent going right" << std::endl;
	}
	else
	{
		influence = ACTIONS::A_NONE;
		if (AGENTQLEARNING_DEBUG)
			std::cout << "I'M DOING NOTHING RIGHT NOW" << std::endl;
	}
	this->body->setInfluence(influence);
}