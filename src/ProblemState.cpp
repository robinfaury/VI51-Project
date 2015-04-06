#include "ProblemState.h"

ProblemState::ProblemState(int id) : m_id(id)
{

}

ProblemState::~ProblemState()
{

}

std::vector<std::string>* ProblemState::getPossibleActions()
{
	return &m_possibleActions;
}

int ProblemState::getId()
{
	return m_id;
}