#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include "ProblemStore.h"


class Problem
{
public:
    Problem();
    ~Problem();

	std::vector<std::string>* getPossibleActions(ProblemState* state);

    ProblemState* getState(int stateId);
    ProblemState* getRandomState();

    ProblemStore* getProblemStore();

	ProblemState* takeAction(ProblemState* pOriginalState, std::string pAction, float& reward);	// Returns the resulting state of doing action pAction in state originalState, giving the appropriate reward

protected:
	ProblemStore m_problemStore;
	std::vector<ProblemState*> m_problemStates;

};

#endif // PROBLEM_H
