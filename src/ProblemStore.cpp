#include "ProblemStore.h"

ProblemStore::ProblemStore()
{

}

ProblemStore::~ProblemStore()
{

}

// Gest best action based on the qvalues of the given state
std::string ProblemStore::getBestAction(int stateId)
{
	// TODO : implement that
	return "";
}

std::string ProblemStore::getBestAction(ProblemState* problemState)  // For the given state, returns the best possible action (the one with highest QValue)
{
	return getBestAction(problemState->getId());
}


// For this state and action, return the associated QValue
float ProblemStore::getQValue(int stateId, std::string action)
{
	// TODO : implement that
	return 0.0f;
}

float ProblemStore::getQValue(ProblemState* problemState, std::string action)    // For this state and action, return the associated QValue
{
	return getQValue(problemState->getId(), action);
}


void ProblemStore::updateQValue(int stateId, std::string action, float newQValue)
{
	// TODO : implement that
}

void ProblemStore::updateQValue(ProblemState* problemState, std::string action, float newQValue)
{
	updateQValue(problemState->getId(), action, newQValue);
}