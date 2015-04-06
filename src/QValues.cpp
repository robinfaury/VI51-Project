#include "Qvalues.h"

QValues::QValues()
{
	srand(static_cast <unsigned> (time(0)));
}

QValues::~QValues()
{

}

void QValues::QValuesAlgorithm(Problem& problem, ProblemState* startingState, int iterations, float learningRate, float discountRate, float actionRandomness, float walkLength)
{
	// Checking for bad values. If there are, set all rates to default
	checkParameterValues(&learningRate, &discountRate, &actionRandomness, &walkLength);

	ProblemState* currentState;

	// Init starting state
	if (startingState == NULL)
		currentState = problem.getRandomState();
	else
		currentState = startingState;

	for (int i = 0; i < iterations; ++i)
	{
		// Randomly chosing to start over from a new state
		if (nextRandomCoefficient() < walkLength)
			currentState = problem.getRandomState();

		// Getting possible actions for this state
		std::vector<std::string>* possibleActions = currentState->getPossibleActions();

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
}


float QValues::nextRandomCoefficient()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void QValues::checkParameterValues(float* learningRate, float* discountRate, float * actionRandomness, float * walkLength)
{
	if (*learningRate < 0.0f || *learningRate > 1.0f)
	{
		//TODO: raise error
		*learningRate = PARAMETER_LEARNING;
	}
	if (*discountRate < 0.0f || *discountRate > 1.0f)
	{
		//TODO: raise error
		*discountRate = PARAMETER_DISCOUNT;
	}
	if (*actionRandomness < 0.0f || *actionRandomness > 1.0f)
	{
		//TODO: raise error
		*actionRandomness = PARAMETER_RANDOMNESS;
	}
	if (*walkLength < 0.0f || *walkLength > 1.0f)
	{
		//TODO: raise error
		*walkLength = PARAMETER_WALK;
	}
}

std::string QValues::getOneOf(std::vector<std::string>* possibleActions)
{
	if (possibleActions->empty())
		return NULL;

	//TODO: 
	return (*possibleActions)[rand() % possibleActions->size()];
}