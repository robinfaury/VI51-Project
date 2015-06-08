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
	int iterations;
	float alpha, gamma, rho, nu;

	// get number of iterations and alpha, gamma, rho and nu parameters from user input
	std::cout << "Enter the number of iterations:" << std::endl;
	std::cin >> iterations;
	std::cout << "Enter the learning rate (alpha) between 0 and 1:" << std::endl;
	std::cin >> alpha;
	std::cout << "Enter the discount rate (gamma) between 0 and 1:" << std::endl;
	std::cin >> gamma;
	std::cout << "Enter the randomness of exploration (rho) between 0 and 1:" << std::endl;
	std::cin >> rho;
	std::cout << "Enter the lenght of walk (nu) between 0 and 1:" << std::endl;
	std::cin >> nu;

	// init problemStates
	Problem problem;
	problem.initProblemStates(this->currentWorld);

	// do the QLearning
	QValues qValues;
	qValues.QValuesAlgorithm(problem, NULL, iterations, alpha, gamma, rho, nu);

	return true;
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