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
	Perception* perception = this->body->getPerception();
	ProblemState* state = m_problem->convertPerceptionToState(perception, false);
	std::string action;
	if (state == NULL)
	{
		std::vector<std::string> actions;
		actions.push_back("left");
		actions.push_back("right");
		actions.push_back("down");
		action = QValues::getOneOf(&actions);
	}
	else
	{
		action = m_problem->getProblemStore()->getBestAction(state);
	}
	ACTIONS influence;
	if (action == "down")
	{
		influence = ACTIONS::A_DOWN;
	}
	else if (action == "left")
	{
		influence = ACTIONS::A_LEFT;
	}
	else if (action == "right")
	{
		influence = ACTIONS::A_RIGHT;
	}
	else
	{
		influence = ACTIONS::A_NONE;
	}
	this->body->setInfluence(influence);
}