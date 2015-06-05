#include "LearningMethod.h"

LearningMethod::LearningMethod(void)
{

}

LearningMethod::LearningMethod(World* world) : currentWorld(world)
{

}

//! Generates a report of the learning.
/**
*   When the learning is complete, this function creates a report containing information about the result, stats, etc.
*/
std::string LearningMethod::generateReport()
{
	return string("DEFAULT REPORT");
}