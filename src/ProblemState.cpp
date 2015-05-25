#include "ProblemState.h"

ProblemState::ProblemState(int id) : m_id(id)
{

}

ProblemState::~ProblemState()
{

}

int ProblemState::getId()
{
	return m_id;
}