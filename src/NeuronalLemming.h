#ifndef NEURONALLEMMING_H_
#define NEURONALLEMMING_H_

#include "Agent.h"
#include "NeuronalNetwork/NeuronalNetworkMonocouche.h"

class NeuronalLemming : public Agent
{
private:
	NeuronalNetworkMonocouche* ArtificialInteligence;
	glm::vec2 lastPos;
	bool victory;

	void move(int id);

public:
	NeuronalLemming(Body* body = NULL);

	void setBrain(NeuronalNetworkMonocouche* ArtificialInteligence);

	virtual void live();

};

#endif
