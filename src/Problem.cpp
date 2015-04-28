#include "Problem.h"

Problem::Problem()
{

}

Problem::~Problem()
{

}

ProblemState* Problem::getState(int stateId)
{
	return m_problemStates[stateId];
}

ProblemState* Problem::getRandomState()
{
	int randomIndex = rand() % m_problemStates.size();
	return m_problemStates[randomIndex];
}

ProblemStore* Problem::getProblemStore()
{
	return &this->m_problemStore;
}

// Returns the resulting state of doing action pAction in state originalState
ProblemState* Problem::takeAction(ProblemState* pOriginalState, std::string pAction, float& reward)
{
	//TODO: implement that
	return NULL;
}

std::vector<std::string>* Problem::getPossibleActions(ProblemState* state)
{
	return this->getProblemStore()->getPossibleActions(state);
}