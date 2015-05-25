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

ProblemState*  Problem::convertPerceptionToState(Perception* perception)
{
	/*
	Tile[8] tiles = perception.getTiles();
	DIRECTION goalDir = perception.getGoalDirection();

	int stateId = 0;
	int leftVal = 0, rightVal = 0, bottomVal = 0, goalVal = 0;

	// process tile on the left
	switch (tiles[3].getType())
	{
		case TILE_TYPE.EMPTY:
		case TILE_TYPE.DIGGABLE:
			leftVal = 0;
			break;
		case TILE_TYPE.UNDIGGABLE:
			leftVal = 1;
			break;
		//case TILE_TYPE.DANGEROUS:
			//leftVal = 2;
			//break;
	}
	int leftPoss = 2;  // total number of possible values for the left tile (leftVal)

	// process tile on the right
	switch (tiles[4].getType())
	{
		case TILE_TYPE.EMPTY:
		case TILE_TYPE.DIGGABLE:
			rightVal = 0;
			break;
		case TILE_TYPE.UNDIGGABLE:
			rightVal = 1;
			break;
		//case TILE_TYPE.DANGEROUS:
			//rightVal = 2;
			//break;
	}
	int rightPoss = 2; // total number of possible values for right tile (rightVal)

	// process tile below
	switch (tiles[6].getType())
	{
		case TILE_TYPE.EMPTY:
			bottomVal = 0;
			break;
		case TILE_TYPE.DIGGABLE:
			bottomVal = 1;
			break;
		case TILE_TYPE.UNDIGGABLE:
			bottomVal = 2;
			break;
		//case TILE_TYPE.DANGEROUS:
			//bottomVal = 3;
			//break;
	}
	int bottomPoss = 3; // total number of possible values for the bottom tile (bottomVal)

	// process goal direction
	switch (goalDir)
	{
		case DIRECTION.LEFT:
			goalVal = 0;
			break;
		case DIRECTION.RIGHT:
			goalVal = 1;
			break;
	}

	stateId = leftPoss * rightPoss * bottomPoss * goalVal + rightPoss * bottomPoss * leftVal + bottomPoss * rightVal + bottomVal;

	return m_problemStates.at(stateId);
	*/
	return NULL;
}

std::vector<std::string>* Problem::getPossibleActions(ProblemState* state)
{
	return this->getProblemStore()->getPossibleActions(state);
}