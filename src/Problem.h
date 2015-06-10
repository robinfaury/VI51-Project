#ifndef PROBLEM_H
#define PROBLEM_H

#define TEMPORARY_MAP_SIZE 40

#include <vector>

#include "PerceptionCross.h"
#include "Perception.h"
#include "ProblemStore.h"
#include "World.h"


class Problem
{
public:
	Problem();
	Problem(World* world);
    ~Problem();

	bool initProblemStore();

	std::vector<std::string>* getPossibleActions(ProblemState* state);

    ProblemState* getState(int stateId);
    ProblemState* getRandomState();

    ProblemStore* getProblemStore();

	ProblemState* takeAction(ProblemState* pOriginalState, std::string pAction, float& reward);	// Returns the resulting state of doing action pAction in state originalState, giving the appropriate reward

	int convertPerceptionToStateId(Perception* perception);
	ProblemState* convertPerceptionToState(Perception* perception, bool createState);

	World* getWorld();
	Body* getBody();
	void setBody(Body* body);

protected:
	ProblemStore m_problemStore;
	std::map<int, ProblemState*> m_problemStates;
	World* m_world;
};

#endif // PROBLEM_H
