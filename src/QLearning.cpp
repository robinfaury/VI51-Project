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
	float alpha = PARAMETER_LEARNING, gamma = PARAMETER_DISCOUNT, rho = PARAMETER_RANDOMNESS, nu = PARAMETER_WALK;
	float floatChoice;
	char choice;

	// get number of iterations and alpha, gamma, rho and nu parameters from user input
    HelperFunctions::safeChoice("Enter the number of iterations :", "Please enter a valid int", iterations);


	do
	{
        HelperFunctions::safeChoice("Do you want to choose the parameter values ? (y/n)", "Please enter 'y' or 'n'", choice);
	} while (choice != 'y' && choice != 'n');

	if (choice == 'y')
	{
        std::string alphaText;
        alphaText = "Enter the learning rate (alpha) between 0 and 1 (default is " + std::to_string(PARAMETER_LEARNING) + ") :";
        HelperFunctions::safeChoice(alphaText, "Please enter a valid float number", alpha);

		std::string gammaText;
        gammaText = "Enter the discount rate (gamma) between 0 and 1 (default is " + std::to_string(PARAMETER_DISCOUNT) + ") :";
        HelperFunctions::safeChoice(gammaText, "Please enter a valid float number", gamma);

        std::string rhoText;
        rhoText = "Enter the randomness of exploration (rho) between 0 and 1 (default is " + std::to_string(PARAMETER_RANDOMNESS) + ") :";
        HelperFunctions::safeChoice(rhoText, "Please enter a valid float number", rho);

        std::string nuText;
        nuText = "Enter the length of walk (nu) between 0 and 1 (default is " + std::to_string(PARAMETER_WALK) + ") :";
        HelperFunctions::safeChoice(nuText, "Please enter a valid float number", nu);
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

	std::cout << "problem qvalues algo done" << std::endl;
	report = m_problem->getProblemStore()->getQValuesReport();
	std::cout << report << std::endl;

	m_learningComplete = true;
	return true;
}

bool QLearning::learn(int iterations, float alpha, float gamma, float rho, float nu)
{
	if (!m_problem->initProblemStore())
	{
		return false;
	}

	QValues qValues;
	if (!qValues.QValuesAlgorithm((*m_problem), NULL, iterations, alpha, gamma, rho, nu))
	{
		return false;
	}
	m_learningComplete = true;
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

// Loading and saving learning results
void QLearning::serialize(pugi::xml_node* node)
{
	if (learningComplete())
	{
		this->m_problem->serialize(node);
	}
	else
	{
		std::cout << "QLearning::serialize : can't serialize problem, learning isn't complete" << std::endl;
	}
}

void QLearning::unzerialize(pugi::xml_node* node)
{
	this->m_problem->unserialize(node);
	this->m_learningComplete = true;
}