#include "Neurone.h"


Neurone::Neurone(void)
{
}

Neurone::Neurone(int nbInput)
{
	while (nbInput--)
		this->poids.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

float Neurone::activeNeurone(std::vector<float>* input)
{
	float sum = 0.0f;
	for (unsigned int i=0; i<input->size(); ++i)
		sum += (*input)[i]*this->poids[i];
	sum += -1 * sigmoide(0);

	return (sigmoide(sum) >= sigmoide(0))? 1.0f : 0.0f;
}

void Neurone::learn(std::vector<float>* sampleData, int out)
{
	float s = activeNeurone(sampleData);
	for (unsigned int i=0; i < this->poids.size(); ++i)
		this->poids[i] += 1.0f*(out - s)*(*sampleData)[i];
}


Neurone::~Neurone(void)
{
}
