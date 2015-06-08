#ifndef QLEARNING_H_
#define QLEARNING_H_

#include "LearningMethod.h"
#include "Problem.h"
#include "QValues.h"

class QLearning : public LearningMethod
{
    public:
    QLearning(World* world);

	//! Performs the actual learning.
	/**
	* The learn function will be executed once by the Simulator. If it finished correctly, learningComplete will return true.
	* This function can use the World, but can't modify it.
	* returns true if the learning finished correctly
	*/
	virtual	bool learn();

	//! Returns true if the learning has finished correctly.
	virtual bool learningComplete();    // Returns true if learning is complete

	//! Creates and returns an agent of the appropriate type, and setup correctly, to use the learning that has been done.
	/**
	*   When the learning is complete (not before!), this function returns an agent. The agent contains the necessary code to use the performed learning.
	*/
	virtual Agent* createAgent();   // Creates and returns an agent set up to be simulated with what has been learned.

	//! Generates a report of the learning.
	/**
	*   When the learning is complete, this function creates a report containing information about the result, stats, etc.
	*/
	virtual std::string generateReport();
};

#endif
