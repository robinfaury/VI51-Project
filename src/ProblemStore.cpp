#include "ProblemStore.h"

ProblemStore::ProblemStore()
{

}

ProblemStore::~ProblemStore()
{

}

bool ProblemStore::initQValues(std::map<int, ProblemState*> problemStates)
{
	if (problemStates.empty())
	{
		return false;
	}
	std::map<std::string, float> actionsQvalues;
	actionsQvalues.insert(std::pair<std::string, float>("left", 0.0f));
	actionsQvalues.insert(std::pair<std::string, float>("right", 0.0f));
	actionsQvalues.insert(std::pair<std::string, float>("down", 0.0f));

	std::map<int, ProblemState*>::iterator it;
	for (it = problemStates.begin(); it != problemStates.end(); ++it)
	{
		m_QValues[it->first] = actionsQvalues;
	}

	return true;
} 

std::vector<std::string>* ProblemStore::getPossibleActions(int stateId)
{
	std::vector<std::string>* actions = new std::vector<std::string>();
	for (std::map<std::string, float>::iterator it = m_QValues[stateId].begin(); it != m_QValues[stateId].end(); ++it)
	{
		actions->push_back(it->first);
	}
	return actions;
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

std::string ProblemStore::getQValuesReport()
{
	std::string report = "----- QValues -----\n";
	std::map <int, std::map<std::string, float> >::iterator it;
	for (it = m_QValues.begin(); it != m_QValues.end(); ++it)
	{
		std::map<std::string, float>::iterator it2;
		report += "state " + std::to_string((*it).first) + "\n";
		for (it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
		{
			report += "\taction : " + (*it2).first + "\n";
			report += "\tQvalue : " + std::to_string((*it2).second) + "\n";
		}
	}
	return report;
}