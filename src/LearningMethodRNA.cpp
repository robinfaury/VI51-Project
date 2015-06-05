#include "LearningMethodRNA.h"

LearningMethodRNA::LearningMethodRNA(void)
{

}

LearningMethodRNA::LearningMethodRNA(World* world, std::string filename) : currentWorld(world)
{
	this->ArtificialInteligence = new NeuronalNetworkMonocouche(filename);
	this->learningFinish = false;
}

bool LearningMethodRNA::learn()
{
	this->ArtificialInteligence->learn();
	return true;
}

bool LearningMethodRNA::learningComplete()
{
	return this->learningFinish;
}

Agent* LearningMethodRNA::createAgent()
{
	NeuronalLemming* a = new NeuronalLemming();
	a->setBrain(this->ArtificialInteligence);
	return a;
}


//! Generates a report of the learning.
/**
*   When the learning is complete, this function creates a report containing information about the result, stats, etc.
*/
std::string LearningMethodRNA::generateReport()
{
	return string("DEFAULT REPORT");
}

LearningMethodRNA::~LearningMethodRNA()
{
	delete this->ArtificialInteligence;
}