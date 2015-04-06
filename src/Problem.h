#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include "ProblemStore.h"


class Problem
{
public:
    Problem();
    ~Problem();

    ProblemState* getState(int stateId);
    ProblemState* getRandomState();

    ProblemStore* getProblemStore();

	ProblemState* takeAction(ProblemState* pOriginalState, std::string pAction, float& reward);	// Returns the resulting state of doing action pAction in state originalState, giving the appropriate reward

protected:
    
};

#endif // PROBLEM_H
