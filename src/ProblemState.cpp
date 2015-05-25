#include "ProblemState.h"

enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum TYLE_TYPE
{
	EMPTY,
	DIGGABLE,
	UNDIGGABLE
};

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