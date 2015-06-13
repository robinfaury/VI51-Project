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
	clearProblem();
	m_world = NULL;
}

void Problem::clearProblem()
{
	for (std::map<int, ProblemState*>::iterator it = this->m_problemStates.begin(); it != this->m_problemStates.end(); ++it)
	{
		delete(it->second);
		it->second = NULL;
	}
	this->m_problemStates.clear();
	this->m_problemStore.clearProblemStore();
}

World* Problem::getWorld()
{
	return m_world;
}

bool Problem::initProblemStore()
{
	if (m_world != NULL)
	{
		if (!initProblemStates())
		{
			std::cout << "ERROR : Problem::initProblemStore : couldn't initialize problem states" << std::endl;
		}

		// Problem store
		if (!this->getProblemStore()->initQValues(this->m_problemStates))
		{
			return false;
		}
		return true;
	}
	return false;
}

bool Problem::initProblemStates()
{
	if (m_world != NULL)
	{
		// States creation
		Perception* perception;
		int stateId;

		for (int x = 1; x < this->m_world->getSize() - 1; x++)
		{
			for (int y = 1; y < this->m_world->getSize() - 1; y++)
			{
				if (m_world->checkValidPosition(x, y))
				{
					perception = m_world->getPerceptionFromTile(x, y);
					stateId = this->convertPerceptionToStateId(perception, this->m_world->getSize());
					if (stateId == -1)
					{
						return false;
					}
					ProblemState* state = new ProblemState(stateId);
					m_problemStates[stateId] = state;
				}
			}
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
		randomX = rand() % ((this->m_world->getSize() - 1) - 1) + 1;
		randomY = rand() % ((this->m_world->getSize() - 1) - 1) + 1;
	} while (!m_world->checkValidPosition(randomX, randomY));

	m_world->resetMap();
	m_world->forceLemmingPosition(randomX, randomY);
	m_world->setPerceptions();

	std::vector<Body*>* bodies = m_world->getBodies();

	if (bodies->empty())
	{
		std::cout << "bodies empty" << std::endl;
		return NULL;
	}
	if (bodies->at(0) == NULL)
	{
		std::cout << "body null" << std::endl;
		return NULL;
	}
	Perception* perception = bodies->at(0)->getPerception();
	ProblemState* state = this->convertPerceptionToState(perception, true);
	return state;
}

ProblemStore* Problem::getProblemStore()
{
	return &this->m_problemStore;
}

// Returns the resulting state of doing action pAction in state originalState
ProblemState* Problem::takeAction(ProblemState* pOriginalState, std::string pAction, float& reward, bool& victory)
{
	victory = false;
	std::vector<Body*>* bodies = m_world->getBodies();

	if (!bodies->empty() && bodies->at(0) != NULL)
	{
		Body* body = bodies->at(0);
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
		int exitX = body->getPerception()->getExitX();
		int exitY = body->getPerception()->getExitY();
		int lemmingX = body->getPosition().at(0);
		int lemmingY = body->getPosition().at(1);*/

		float exitDistance = std::abs(body->getPerception()->getExitX() - body->getPerception()->getLemmingX()) + std::abs(body->getPerception()->getExitY() - body->getPerception()->getLemmingY());
		if (QLEARNING_DEBUG)
			std::cout << "Problem::takeAction : exitDistance : " << exitDistance << std::endl;

		// Updating world
		body->setInfluence(influence);
		m_world->collectInfluences();
		m_world->resolveInfluences();
		m_world->setPerceptions();

		// Getting lemming position
		std::vector<int> pos = bodies->at(0)->getPosition();
		int xPos = pos.at(0);
		int yPos = pos.at(1);

		// Percieving new state
		Perception* newPerception = m_world->getPerceptionFromTile(xPos, yPos);
		ProblemState* newState = convertPerceptionToState(newPerception, true);

		// Compute new exit distance

		float newExitDistance = std::abs(newPerception->getExitX() - newPerception->getLemmingX()) + std::abs(newPerception->getExitY() - newPerception->getLemmingY());
		if (QLEARNING_DEBUG)
			std::cout << "Problem::takeAction : newExitDistance : " << newExitDistance << std::endl;

		// If both distance are the same : then the action had no effect. Give negative reward
		if (exitDistance == newExitDistance)
		{
			//reward = -(BASE_REWARD/8);
		}
		else if (newExitDistance == 0)	// Victory
		{
			reward = 2 * BASE_REWARD;
			victory = true;
		}
		else // calculate and set reward using manhattan distance
		{
			//reward = (exitDistance - newExitDistance) * BASE_REWARD/4;
			reward = 0.0f;
		}

		if (QLEARNING_DEBUG)
		{
			std::cout << "position : " << xPos << ", " << yPos << std::endl;
			std::cout << "reward : " << reward << std::endl;
		}



		return newState;
	}
	return NULL;
}

int Problem::convertPerceptionToStateId(Perception* perception, int worldSize)
{
	if (perception == NULL)
	{
		std::cout << "ERROR : Problem::convertPerceptionToStateId : perception NULL !!" << std::endl;
		return -1;
	}
	std::vector<PhysicalObject*>* objects = perception->getPerceivedObjects();

	//int goalPosX = perception->getExitX();
	int actualPosX = perception->getLemmingX();
	int actualPosY = perception->getLemmingY();

	int worldPart; // represents a part of the world when it is divided in 4
	if (actualPosX <= std::abs(worldSize / 2)) // if lemming in the left part of world
	{
		if (actualPosY <= std::abs(worldSize / 2)) // if lemming in the upper part of world
		{
			worldPart = 0;
		}
		else // if lemming in the lower part of world
		{
			worldPart = 2;
		}
	}
	else // if lemming in the right part of world
	{
		if (actualPosY <= std::abs(worldSize / 2)) // if lemming in the upper part of world
		{
			worldPart = 1;
		}
		else // if lemming in the lower part of world
		{
			worldPart = 3;
		}
	}
	int worldPartPoss = 4; // total number of possible values for the world part

	// process goal direction
	/*DIRECTION goalDir;
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
	int goalPoss = 3;  // total number of possible values for the goal direction*/

	// process tile on the left
	TILE_TYPE leftTile;
	if (objects->at(3) == NULL)
		leftTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
	else
	{
		switch (objects->at(3)->getSemantic())
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
	}

	int leftPoss = 2;  // total number of possible values for the left tile

	// process tile on the right
	TILE_TYPE rightTile;
	if (objects->at(4) == NULL)
		rightTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
	else
	{
		switch (objects->at(4)->getSemantic())
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
	}

	int rightPoss = 2; // total number of possible values for right tile

	// process tile below
	TILE_TYPE bottomTile;
	if (objects->at(6) == NULL)
		bottomTile = TILE_TYPE::EMPTY_OR_DIGGABLE;
	else
	{
		switch (objects->at(6)->getSemantic())
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
	}

	// calculate unique state id
	//int stateId = goalDir + leftTile * goalPoss + rightTile * (goalPoss + leftPoss) + bottomTile * (goalPoss * leftPoss * rightPoss);
	//int stateId = leftTile + rightTile * (leftPoss) + bottomTile * (leftPoss * rightPoss);
	int stateId = worldPart + leftTile * worldPartPoss + rightTile * (worldPartPoss + leftPoss) + bottomTile * (worldPartPoss * leftPoss * rightPoss);

	return stateId;
}

ProblemState* Problem::convertPerceptionToState(Perception* perception, bool createState)
{
	int stateId = this->convertPerceptionToStateId(perception, this->m_world->getSize());
	if (stateId == -1)
	{
		return NULL;
	}
	if (m_problemStates[stateId] == NULL && createState)
	{
		std::cout << "ERROR : Creating new state" << std::endl;
		std::cout << "ERROR : error position " << perception->getLemmingX() << ", " << perception->getLemmingY() <<   std::endl;
		ProblemState* state = new ProblemState(stateId);
		m_problemStates[stateId] = state;
	}
	return m_problemStates[stateId];
}

std::vector<std::string>* Problem::getPossibleActions(ProblemState* state)
{
	return this->getProblemStore()->getPossibleActions(state);
}

// Load / save
// Saves all problem states, and associated QValues in the store
void Problem::serialize(pugi::xml_node* node)
{
	this->m_problemStore.serialize(node);
}

// Loads problem states and associated QValues from given node
void Problem::unserialize(pugi::xml_node* node)	
{
	this->m_problemStates.clear();
	this->m_problemStore.unserialize(node);
}