#include "Problem.h"

Problem::Problem()
{

}

Problem::Problem(Body* body, World* world)
{
	m_body = body;
	m_world = world;
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
	ACTIONS influence;
	if (pAction == "down")
	{
		influence = ACTIONS::A_DOWN;
	}
	else if (pAction == "left")
	{
		influence = ACTIONS::A_LEFT;
	}
	else if (pAction == "right")
	{
		influence = ACTIONS::A_RIGHT;
	}
	else
	{
		influence = ACTIONS::A_NONE;
	}

	m_body->setInfluence(influence);

	// TODO: process influences, resolve actions and send back new perception

	Perception* newPerception = new PerceptionCross();
	ProblemState* newState = convertPerceptionToState(newPerception);

	// TODO: calculate and set reward

	return newState;
}

ProblemState* Problem::convertPerceptionToState(Perception* perception)
{
	std::vector<PhysicalObject*>* objects = perception->getPerceivedObjects();

	int goalPosX = perception->getExitX();
	int actualPosX = perception->getLemmingX();

	// process goal direction
	DIRECTION goalDir;
	if (actualPosX > goalPosX)
	{
		goalDir = DIRECTION::LEFT;
	}
	else if (actualPosX < goalPosX)
	{
		goalDir = DIRECTION::RIGHT;
	}
	else
	{
		goalDir = DIRECTION::DOWN;
	}
	int goalPoss = 3;  // total number of possible values for the goal direction

	// process tile on the left
	TILE_TYPE leftTile;
	switch (objects->at(3)->getSemantic())
	{
		case SEMANTIC::T_ROCK:
		case SEMANTIC::B_LEMMING: 
			leftTile = TILE_TYPE::UNDIGGABLE;
			break;
		default:
			leftTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
			break;
	}
	int leftPoss = 2;  // total number of possible values for the left tile

	// process tile on the right
	TILE_TYPE rightTile;
	switch (objects->at(4)->getSemantic())
	{
		case SEMANTIC::T_ROCK:
		case SEMANTIC::B_LEMMING:
			rightTile = TILE_TYPE::UNDIGGABLE;
			break;
		default:
			rightTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
			break;
	}
	int rightPoss = 2; // total number of possible values for right tile

	// process tile below
	TILE_TYPE bottomTile;
	switch (objects->at(6)->getSemantic())
	{
		case SEMANTIC::T_ROCK:
		case SEMANTIC::B_LEMMING:
			bottomTile = TILE_TYPE::UNDIGGABLE;
			break;
		default:
			bottomTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
			break;
	}

	// calculate unique state id
	int stateId = goalDir + leftTile * goalPoss + rightTile * (goalPoss + leftPoss) + bottomTile * (goalPoss * leftPoss * rightPoss); 

	return m_problemStates.at(stateId);
}

std::vector<std::string>* Problem::getPossibleActions(ProblemState* state)
{
	return this->getProblemStore()->getPossibleActions(state);
}