#include "QValues.h"

QValues::QValues()
{
	srand(static_cast <unsigned> (time(0)));
}

QValues::~QValues()
{

}

bool QValues::QValuesAlgorithm(Problem& problem, ProblemState* startingState, int iterations, float learningRate, float discountRate, float actionRandomness, float walkLength)
{
	// Checking for bad values. If there are, set all rates to default
	if (!checkParameterValues(&learningRate, &discountRate, &actionRandomness, &walkLength))
	{
		return false;
	}

	std::cout << "parameter values chacked" << std::endl;

	ProblemState* currentState;

	// Init starting state
	if (startingState == NULL)
	{
		World* world = problem.getWorld();
		currentState = problem.getRandomState();
		if (currentState == NULL)
		{
			std::cout << "state NULL !!" << std::endl;
			return false;
		}
		std::cout << "starting state set" << std::endl;
	}
	else
	{
		currentState = startingState;
	}
		
	for (int i = 0; i < iterations; ++i)
	{
		std::cout << "iteration " << i << std::endl;

		// Randomly chosing to start over from a new state
		if (nextRandomCoefficient() < walkLength)
			currentState = problem.getRandomState();

		// Getting possible actions for this state
		std::vector<std::string>* possibleActions = problem.getPossibleActions(currentState);

		std::string action;
		// Randomly chosing wheter to act randomly, or select the best action
		if (nextRandomCoefficient() < actionRandomness)
		{
			// Random action
			action = getOneOf(possibleActions);
		}
		else
		{
			// Getting best possible action
			action = problem.getProblemStore()->getBestAction(currentState);
		}

		// Resolve action
		float reward = 0.0f;
		ProblemState* newState = problem.takeAction(currentState, action, reward);

		float qValue = problem.getProblemStore()->getQValue(currentState, action);	// Getting qValue for the action taken
		float maxQValue = problem.getProblemStore()->getQValue(newState,
			problem.getProblemStore()->getBestAction(newState));	// Getting best q value of the new state

		// QLearning
		qValue = (1 - learningRate) * qValue
			+ learningRate * (reward + discountRate * maxQValue);

		problem.getProblemStore()->updateQValue(currentState, action, qValue);

		currentState = newState;
	}
	return true;
}


float QValues::nextRandomCoefficient()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

bool QValues::checkParameterValues(float* learningRate, float* discountRate, float * actionRandomness, float * walkLength)
{
	if (*learningRate < 0.0f || *learningRate > 1.0f)
	{
		std::cout << "error: learning rate out of range" << std::endl;
		return false;
	}
	if (*discountRate < 0.0f || *discountRate > 1.0f)
	{
		std::cout << "error: discount rate out of range" << std::endl;
		return false;
	}
	if (*actionRandomness < 0.0f || *actionRandomness > 1.0f)
	{
		std::cout << "error: action randomness rate out of range" << std::endl;
		return false;
	}
	if (*walkLength < 0.0f || *walkLength > 1.0f)
	{
		std::cout << "error: walk length rate out of range" << std::endl;
		return false;
	}
	return true;
}

std::string QValues::getOneOf(std::vector<std::string>* possibleActions)
{
	if (possibleActions->empty())
		return NULL;

	return (*possibleActions)[rand() % possibleActions->size()];
}
