#include "ProblemStore.h"

ProblemStore::ProblemStore()
{

}

ProblemStore::~ProblemStore()
{

}

std::vector<std::string>* ProblemStore::getPossibleActions(int stateId)
{
    //TODO: INCOMMING SEGFAULT - change that!
	std::vector<std::string> actions;
	for (std::map<std::string, float>::iterator it = m_QValues[stateId].begin(); it != m_QValues[stateId].end(); ++it)
	{
		actions.push_back(it->first);
	}
	return &actions;
}

std::vector<std::string>* ProblemStore::getPossibleActions(ProblemState* state)
{
	return getPossibleActions(state->getId());
}

// Gest best action based on the qvalues of the given state
std::string ProblemStore::getBestAction(int stateId)
{
	std::map<std::string, float>::iterator it;
	it = m_QValues[stateId].begin();
	std::string bestAction = it->first;
	float bestValue = it->second;
	while (it != m_QValues[stateId].end())
	{
		if (it->second > bestValue)
		{
			bestValue = it->second;
			bestAction = it->first;
		}
		it++;
	}
	return bestAction;
}

std::string ProblemStore::getBestAction(ProblemState* problemState)  // For the given state, returns the best possible action (the one with highest QValue)
{
	return getBestAction(problemState->getId());
}


// For this state and action, return the associated QValue
float ProblemStore::getQValue(int stateId, std::string action)
{
	float qValue = m_QValues[stateId][action];
	return qValue;
}

float ProblemStore::getQValue(ProblemState* problemState, std::string action)    // For this state and action, return the associated QValue
{
	return getQValue(problemState->getId(), action);
}


void ProblemStore::updateQValue(int stateId, std::string action, float newQValue)
{
	m_QValues[stateId][action] = newQValue;
}

void ProblemStore::updateQValue(ProblemState* problemState, std::string action, float newQValue)
{
	updateQValue(problemState->getId(), action, newQValue);
}
