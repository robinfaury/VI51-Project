#ifndef NEURONALNETWORKMONOCOUCHE_H_
#define NEURONALNETWORKMONOCOUCHE_H_

#include <time.h>

#include "Neurone.h"
#include "Parser.h"
#include "../LearningMethod.h"

class NeuronalNetworkMonocouche
{
private:
	std::vector<Neurone*> neurones;
	std::vector<Sample> samples;
	int nbNeurones;

	NeuronalNetworkMonocouche(void);

public:
	NeuronalNetworkMonocouche(std::string filenameSamplingData);

	bool learn();

	void testing();

	std::vector<float> testing(Sample caracteristicVector);

	~NeuronalNetworkMonocouche(void);
};

#endif