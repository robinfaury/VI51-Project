#ifndef NEURONALNETWORKMONOCOUCHE_H_
#define NEURONALNETWORKMONOCOUCHE_H_

#include <time.h>

#include "Neurone.h"
#include "Parser.h"

class NeuronalNetworkMonocouche
{
private:
	std::vector<Neurone*> neurones;
	std::vector<Sample> samples;
	int nbNeurones;

	NeuronalNetworkMonocouche(void);

public:
	NeuronalNetworkMonocouche(std::string filenameSamplingData);

	void learning();
	void testing();

	~NeuronalNetworkMonocouche(void);
};

#endif