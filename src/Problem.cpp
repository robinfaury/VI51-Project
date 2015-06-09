#include "Problem.h"

Problem::Problem()
{

}

Problem::Problem(World* world)
{
	m_world = world;
}

Problem::~Problem()
{
	m_world = NULL;
}

World* Problem::getWorld()
{
	return m_world;
}

bool Problem::initProblemStore()
{
	if (m_world != NULL)
	{
		std::cout << "init problem states" << std::endl;

		Perception* perception;
		int stateId;

		for (int x = 1; x < TEMPORARY_MAP_SIZE - 2; x++)
		{
			for (int y = 1; y < TEMPORARY_MAP_SIZE - 2; y++)
			{
				std::cout << "(" << x << ", " << y << ")" << std::endl;
				if (m_world->checkValidPosition(x, y))
				{
					std::cout << "valid tile" << std::endl;
					perception = m_world->getPerceptionFromTile(x, y);
					stateId = this->convertPerceptionToStateId(perception);
					if (stateId == -1)
					{
						return false;
					}
					ProblemState* state = new ProblemState(stateId);
					m_problemStates[stateId] = state;
					std::cout << "state id: " << state->getId() << std::endl;
				}
			}
		}

		if (!this->getProblemStore()->initQValues(this->m_problemStates))
		{
			return false;
		}
		return true;
	}
	return false;
}

ProblemState* Problem::getState(int stateId)
{
	return m_problemStates[stateId];
}

ProblemState* Problem::getRandomState()
{
	// TODO : getMapSize instead of TEMPORARY_MAP_SIZE
	int randomX, randomY;
	do
	{
		randomX = rand() % ((TEMPORARY_MAP_SIZE - 2) - 1) + 1;
		randomY = rand() % ((TEMPORARY_MAP_SIZE - 2) - 1) + 1;
	} while (!m_world->checkValidPosition(randomX, randomY));

	std::cout << "random pos generated" << std::endl;

	m_world->forceLemmingPosition(randomX, randomY);
	m_world->setPerceptions();

	std::cout << "lemming pos forced" << std::endl;

	std::vector<Body*>* bodies = m_world->getBodies();

	if (bodies->empty())
	{
		std::cout << "bodies empty" << std::endl;
		return false;
	}
	if (bodies->at(0) == NULL)
	{
		std::cout << "body null" << std::endl;
		return false;
	}
	Perception* perception = bodies->at(0)->getPerception();
	std::cout << "get perceptions ok" << std::endl;
	ProblemState* state = this->convertPerceptionToState(perception);
	return state;
}

ProblemStore* Problem::getProblemStore()
{
	return &this->m_problemStore;
}

// Returns the resulting state of doing action pAction in state originalState
ProblemState* Problem::takeAction(ProblemState* pOriginalState, std::string pAction, float& reward)
{
	std::vector<Body*>* bodies = m_world->getBodies();

	if (!bodies->empty() && bodies->at(0) != NULL)
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
		
		bodies->at(0)->setInfluence(influence);
		m_world->collectInfluences();
		m_world->resolveInfluences();
		m_world->setPerceptions();

		std::vector<int> pos = bodies->at(0)->getPosition();
		int xPos = pos.at(0);
		int yPos = pos.at(1);

		Perception* newPerception = m_world->getPerceptionFromTile(xPos, yPos);
		ProblemState* newState = convertPerceptionToState(newPerception);

		// calculate and set reward using manhattan distance
		reward = std::abs(newPerception->getExitX() - newPerception->getLemmingX()) + std::abs(newPerception->getExitX() - newPerception->getLemmingY());

		return newState;
	}
	return NULL;
}

int Problem::convertPerceptionToStateId(Perception* perception)
{
	if (perception == NULL)
	{
		std::cout << "perception NULL !!" << std::endl;
		return -1;
	}
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
	if (objects->at(1) == NULL)
	{
		std::cout << "perception[1] NULL !!" << std::endl;
		return -1;
	}
	switch (objects->at(1)->getSemantic())
	{
		case SEMANTIC::T_ROCK:
		case SEMANTIC::B_LEMMING:
		case SEMANTIC::T_BOUND:
			leftTile = TILE_TYPE::UNDIGGABLE;
			break;
		default:
			leftTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
			break;
	}
	int leftPoss = 2;  // total number of possible values for the left tile

	// process tile on the right
	TILE_TYPE rightTile;
	if (objects->at(2) == NULL)
	{
		std::cout << "perception[2] NULL !!" << std::endl;
		return -1;
	}
	switch (objects->at(2)->getSemantic())
	{
		case SEMANTIC::T_ROCK:
		case SEMANTIC::B_LEMMING:
		case SEMANTIC::T_BOUND:
			rightTile = TILE_TYPE::UNDIGGABLE;
			break;
		default:
			rightTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
			break;
	}
	int rightPoss = 2; // total number of possible values for right tile

	// process tile below
	TILE_TYPE bottomTile;
	if (objects->at(3) == NULL)
	{
		std::cout << "perception[3] NULL !!" << std::endl;
		return -1;
	}
	switch (objects->at(3)->getSemantic())
	{
		case SEMANTIC::T_ROCK:
		case SEMANTIC::B_LEMMING:
		case SEMANTIC::T_BOUND:
			bottomTile = TILE_TYPE::UNDIGGABLE;
			break;
		default:
			bottomTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
			break;
	}

	// calculate unique state id
	int stateId = goalDir + leftTile * goalPoss + rightTile * (goalPoss + leftPoss) + bottomTile * (goalPoss * leftPoss * rightPoss);

	std::cout << "stateId = " << stateId << std::endl;

	return stateId;
}

ProblemState* Problem::convertPerceptionToState(Perception* perception)
{
	int stateId = this->convertPerceptionToStateId(perception);
	if (stateId == -1)
	{
		return NULL;
	}
	return m_problemStates.at(stateId);
}

std::vector<std::string>* Problem::getPossibleActions(ProblemState* state)
{
	return this->getProblemStore()->getPossibleActions(state);
}