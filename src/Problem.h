#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>

#include "PerceptionCross.h"
#include "Perception.h"
#include "ProblemStore.h"
#include "World.h"


class Problem
{
public:
	Problem();
    ~Problem();

	void initProblemStates(World* world);

	std::vector<std::string>* getPossibleActions(ProblemState* state);

    ProblemState* getState(int stateId);
    ProblemState* getRandomState();

    ProblemStore* getProblemStore();

	ProblemState* takeAction(ProblemState* pOriginalState, std::string pAction, float& reward);	// Returns the resulting state of doing action pAction in state originalState, giving the appropriate reward

	int convertPerceptionToStateId(Perception* perception);
	ProblemState* convertPerceptionToState(Perception* perception);

protected:
	ProblemStore m_problemStore;
	std::vector<ProblemState*> m_problemStates;

};

#endif // PROBLEM_H
