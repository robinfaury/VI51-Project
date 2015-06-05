#ifndef NEURONE_H_
#define NEURONE_H_

#include <vector>

#include "MathUtils.h"
#include <stdlib.h>

class Neurone
{
private:
	std::vector<float> poids;

	Neurone();

public:
	Neurone(int nbInput);

	float activeNeurone(std::vector<float>* input);
	void learn(std::vector<float>* sampleData, int out);

	~Neurone(void);
};

#endif
