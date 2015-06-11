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
	this->learningFinish = true;
	return true;
}

bool LearningMethodRNA::learningComplete()
{
	return this->learningFinish;
}

Agent* LearningMethodRNA::createAgent()
{
	if (learningComplete())
	{
		NeuronalLemming* a = new NeuronalLemming();
		a->setBrain(this->ArtificialInteligence);
		return a;
	}
	return NULL;
}


//! Generates a report of the learning.
/**
*   When the learning is complete, this function creates a report containing information about the result, stats, etc.
*/
std::string LearningMethodRNA::generateReport()
{
	return string("DEFAULT REPORT");
}

// Loading and saving learning results
void LearningMethodRNA::serialize(pugi::xml_node* node)
{

}

void LearningMethodRNA::unzerialize(pugi::xml_node* node)
{

}

LearningMethodRNA::~LearningMethodRNA()
{
	delete this->ArtificialInteligence;
}