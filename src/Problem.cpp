#include "Problem.h"

Problem::Problem()
{

}

Problem::~Problem()
{

}

void Problem::initProblemStates(World* world)
{
	std::cout << "init problem states" << std::endl;
	Map* map = world->getMap();
	std::map<std::pair<int, int>, Cell*>* stdmap = map->getMap();
	std::map<std::pair<int, int>, Cell*>::iterator it;
	int xPos, yPos;
	Perception* perception;
	int stateId;
	std::cout << "start parcourt stdmap" << std::endl;
	for (it = stdmap->begin(); it != stdmap->end(); ++it)
	{
		std::cout << "iteration" << std::endl;
		xPos = (*it).first.first;
		yPos = (*it).first.second;
		std::cout << "(" << xPos << ", " << yPos << ")" << std::endl;
		perception = world->getPerceptionFromTile(xPos, yPos);
		std::cout << "perception got" << std::endl;
		stateId = this->convertPerceptionToStateId(perception);
		std::cout << "perception converted" << std::endl;
		ProblemState* state = new ProblemState(stateId);
		m_problemStates[stateId] = state;
		std::cout << "(" << xPos << ", " << yPos << ") state " << state->getId() << std::endl;
	}
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

	/*
	boucle pour x et y
	checkValidPos
	forceLemmingPos
	world->getBodies() -> return 1 lemming
	lemming->setInfluence(influence);
	collectInfluence
	resolveInfuence
	*/

	// TODO: process influences, resolve actions and send back new perception

	Perception* newPerception = new PerceptionCross();
	ProblemState* newState = convertPerceptionToState(newPerception);

	// calculate and set reward using manhattan distance
	reward = std::abs(newPerception->getExitX() - newPerception->getLemmingX()) + std::abs(newPerception->getExitX() - newPerception->getLemmingY());
	
	return newState;
}

int Problem::convertPerceptionToStateId(Perception* perception)
{
	std::cout << "start convert perception" << std::endl;
	if (perception == NULL)
	{
		std::cout << "perception null !!" << std::endl;
	}
	std::vector<PhysicalObject*>* objects = perception->getPerceivedObjects();
	std::cout << "perceived objects got" << std::endl;

	int goalPosX = perception->getExitX();
	int actualPosX = perception->getLemmingX();
	std::cout << "X positions got" << std::endl;

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

	std::cout << "goal dir processed" << std::endl;

	// process tile on the left
	TILE_TYPE leftTile;
	switch (objects->at(1)->getSemantic())
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

	std::cout << "left tile processed" << std::endl;

	// process tile on the right
	TILE_TYPE rightTile;
	switch (objects->at(2)->getSemantic())
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

	std::cout << "right tile processed" << std::endl;

	// process tile below
	TILE_TYPE bottomTile;
	std::cout << objects->size() << std::endl;
	if (objects->at(3) == NULL)
	{
		std::cout << "objects at 3 null !!" << std::endl;
	}
	std::cout << "switch" << std::endl;
	switch (objects->at(3)->getSemantic())
	{
	case SEMANTIC::T_ROCK:
	case SEMANTIC::B_LEMMING:
		bottomTile = TILE_TYPE::UNDIGGABLE;
		break;
	default:
		bottomTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
		break;
	}

	std::cout << "bottom tile processed" << std::endl;

	// calculate unique state id
	int stateId = goalDir + leftTile * goalPoss + rightTile * (goalPoss + leftPoss) + bottomTile * (goalPoss * leftPoss * rightPoss);

	std::cout << "bottom tile processed" << std::endl;

	return stateId;
}

ProblemState* Problem::convertPerceptionToState(Perception* perception)
{
	int stateId = this->convertPerceptionToStateId(perception);
	return m_problemStates.at(stateId);
}

std::vector<std::string>* Problem::getPossibleActions(ProblemState* state)
{
	return this->getProblemStore()->getPossibleActions(state);
}