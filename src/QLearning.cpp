#include "QLearning.h"

QLearning::QLearning(World* world) : LearningMethod(world)
{

}

//! Performs the actual learning.
/**
* The learn function will be executed once by the Simulator. If it finished correctly, learningComplete will return true.
* This function can use the World, but can't modify it.
* returns true if the learning finished correctly
*/
bool QLearning::learn()
{
	//Do the actual learning
	return false;
}

//! Returns true if the learning has finished correctly.
bool QLearning::learningComplete()
{
	//TODO: check that learning occured correctly
	return false;
}

//! Creates and returns an agent of the appropriate type, and setup correctly, to use the learning that has been done.
/**
*   When the learning is complete (not before!), this function returns an agent. The agent contains the necessary code to use the performed learning.
*/
Agent* QLearning::createAgent()
{
	if (learningComplete())
	{
		//TODO: return an agent configured with the learning
	}
	else
		return NULL;
}

//! Generates a report of the learning.
/**
*   When the learning is complete, this function creates a report containing information about the result, stats, etc.
*/
std::string QLearning::generateReport()
{
	//TODO: generate report of the learning
	return LearningMethod::generateReport();
}