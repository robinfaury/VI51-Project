#include "NeuronalLemming.h"

NeuronalLemming::NeuronalLemming(Body* body) : Agent(body)
{

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

	Perception* p = this->body->getPerception();
	std::vector<PhysicalObject*>* objects = p->getPerceivedObjects();
	Sample caracteristicVector;

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
	int id = 0;
	for (int i=1; i<classe.size(); ++i)
	{
		if (classe[i] > hightScore)
		{
			hightScore = classe[i];
			id = i;
		}
	}

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
