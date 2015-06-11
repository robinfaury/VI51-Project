#include "QLearning.h"

QLearning::QLearning(World* world) : LearningMethod(world)
{
	m_learningComplete = false;
	m_problem = new Problem(world);
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
	float alpha = 0.3f, gamma = 0.75f, rho = 0.2f, nu = 0.1f;
	char choice;

	// get number of iterations and alpha, gamma, rho and nu parameters from user input
	std::cout << "Enter the number of iterations:" << std::endl;
	std::cin >> iterations;
	do
	{
		std::cout << "Do you want to choose the parameter values ? (y/n)" << std::endl;
		std::cin >> choice;
	} while (choice != 'y' && choice != 'n');
	if (choice == 'y')
	{
		std::cout << "Enter the learning rate (alpha) between 0 and 1:" << std::endl;
		std::cin >> alpha;
		std::cout << "Enter the discount rate (gamma) between 0 and 1:" << std::endl;
		std::cin >> gamma;
		std::cout << "Enter the randomness of exploration (rho) between 0 and 1:" << std::endl;
		std::cin >> rho;
		std::cout << "Enter the lenght of walk (nu) between 0 and 1:" << std::endl;
		std::cin >> nu;
	}

	// init problemStates
	if (!m_problem->initProblemStore())
	{
		return false;
	}

	std::string report = m_problem->getProblemStore()->getQValuesReport();

	std::cout << "problem store init done" << std::endl;
	std::cout << report << std::endl;

	// do the QLearning
	QValues qValues;
	if (!qValues.QValuesAlgorithm((*m_problem), NULL, iterations, alpha, gamma, rho, nu))
	{
		return false;
	}

	report = m_problem->getProblemStore()->getQValuesReport();
	std::cout << report << std::endl;

	std::cout << "problem qvalues algo done" << std::endl;

	m_learningComplete = true;
	return true;
}

//! Returns true if the learning has finished correctly.
bool QLearning::learningComplete()
{
	return this->m_learningComplete;
}

//! Creates and returns an agent of the appropriate type, and setup correctly, to use the learning that has been done.
/**
*   When the learning is complete (not before!), this function returns an agent. The agent contains the necessary code to use the performed learning.
*/
Agent* QLearning::createAgent()
{
	if (learningComplete())
	{
		Agent* agent = new AgentQLearning(m_problem);
		return agent;
	}
	else
	{
		return NULL;
	}
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