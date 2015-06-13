#include "NeuronalLemming.h"

NeuronalLemming::NeuronalLemming(Body* body) : Agent(body)
{
	this->lastPos = glm::vec2(0, 0);
	this->victory = false;
}

void NeuronalLemming::setBrain(NeuronalNetworkMonocouche* ArtificialInteligence)
{
	this->ArtificialInteligence = ArtificialInteligence;
}

void NeuronalLemming::live()
{
	// If there is no linked body, do nothing
	if (!isLinked())
		return;

	if (this->victory)
		return;
	
	Perception* p = this->body->getPerception();
	std::vector<PhysicalObject*>* objects = p->getPerceivedObjects();
	glm::vec2 lemmingPosition = glm::vec2((*objects)[8]->getPosition()[0], (*objects)[8]->getPosition()[1]);
	Sample caracteristicVector;
	int id = 0;

	for (int i=3; i<8; ++i)
	{
		if ((*objects)[i] == NULL)
			caracteristicVector.addData(0.0f);
		else
		{
			switch ((*objects)[i]->getSemantic())
			{
			case SEMANTIC::T_BOUND:
				caracteristicVector.addData(1.0f);
				break;
			case SEMANTIC::T_DIRT:
				caracteristicVector.addData(0.5f);
				break;
			case SEMANTIC::T_EXIT:
				caracteristicVector.addData(0.0f);
				if (i == 0)
					id = 2;
				else if (i == 1)
					id = 1;
				else if (i == 4)
					id = 3;
				move(id);
				this->victory = true;
				break;
			case SEMANTIC::T_ROCK:
				caracteristicVector.addData(1.0f);
				break;
			default:
				break;
			}
		}
	}

	std::vector<float> classe = this->ArtificialInteligence->testing(caracteristicVector);
	float hightScore = classe[0];
	
	for (int i=1; i<classe.size(); ++i)
	{
		if (classe[i] > hightScore)
		{
			hightScore = classe[i];
			id = i;
		}
	}

	BodyLemmingMemory* bodyMemory = dynamic_cast<BodyLemmingMemory*>(this->body);

	if (bodyMemory != NULL)
	{
		switch (id)
		{
		case 0:
			bodyMemory->getMemory()->visited(lemmingPosition.x, lemmingPosition.y);
			break;
		case 1:
			if (bodyMemory->getMemory()->visited(lemmingPosition.x+1, lemmingPosition.y))
			{
				if (bodyMemory->getMemory()->visited(lemmingPosition.x-1, lemmingPosition.y))
					id = 3;
				else
					if (rand()%100 < 70)
						id = 2;
					else
						id = 3;
			}
			break;
		case 2:
			if (bodyMemory->getMemory()->visited(lemmingPosition.x-1, lemmingPosition.y))
			{
				if (bodyMemory->getMemory()->visited(lemmingPosition.x+1, lemmingPosition.y))
					id = 3;
				else
					if (rand()%100 < 20)
						id = 1;
					else
						id = 3;
			}
			break;
		case 3:
			bodyMemory->getMemory()->visited(lemmingPosition.x, lemmingPosition.y+1);
			break;
		default:
			bodyMemory->getMemory()->visited(lemmingPosition.x, lemmingPosition.y);
			break;
		}
	}

	if (this->lastPos == lemmingPosition)
	{
		if (rand()%100 < 33)
			id = 1;
		else
			if (rand()%100 < 50)
				id = 2;
			else
				id = 3;
	}

	move(id);
	this->lastPos = lemmingPosition;
}

void NeuronalLemming::move(int id)
{
	switch (id)
	{
	case 0:
		this->body->setInfluence(ACTIONS::A_NONE);
		break;
	case 1:
		this->body->setInfluence(ACTIONS::A_RIGHT);
		break;
	case 2:
		this->body->setInfluence(ACTIONS::A_LEFT);
		break;
	case 3:
		this->body->setInfluence(ACTIONS::A_DOWN);
		break;
	default:
		this->body->setInfluence(ACTIONS::A_NONE);
		break;
	}
	
}