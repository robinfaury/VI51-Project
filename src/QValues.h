#ifndef QVALUES_H
#define QVALUES_H

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Problem.h"

#define PARAMETER_LEARNING		0.3f
#define PARAMETER_DISCOUNT		0.75f
#define PARAMETER_RANDOMNESS	0.2f
#define PARAMETER_WALK			0.1f

class QValues
{
public:
    QValues();
    ~QValues();

	// All float values are between 0 and 1
	void QValuesAlgorithm(Problem& problem, ProblemState* startingState, int iterations, 
		float learningRate = PARAMETER_LEARNING, 
		float discountRate = PARAMETER_DISCOUNT, 
		float actionRandomness = PARAMETER_RANDOMNESS, 
		float walkLength = PARAMETER_WALK);

private :
	float nextRandomCoefficient();
	void checkParameterValues(float* learningRate, float* discountRate, float * actionRandomness, float * walkLength);
	std::string getOneOf(std::vector<std::string>* possibleActions);
};

#endif // QVALUES_H
