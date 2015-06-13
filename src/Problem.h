#ifndef PROBLEM_H
#define PROBLEM_H

#define BASE_REWARD 1

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
	void clearProblem();

	bool initProblemStore();
	bool initProblemStates();

	std::vector<std::string>* getPossibleActions(ProblemState* state);

    ProblemState* getState(int stateId);
    ProblemState* getRandomState();

    ProblemStore* getProblemStore();

	ProblemState* takeAction(ProblemState* pOriginalState, std::string pAction, float& reward, bool& victory);	// Returns the resulting state of doing action pAction in state originalState, giving the appropriate reward

	static int convertPerceptionToStateId(Perception* perception, int worldSize);
	ProblemState* convertPerceptionToState(Perception* perception, bool createState);

	World* getWorld();
	Body* getBody();
	void setBody(Body* body);

	// Load / save
	void serialize(pugi::xml_node* node);	// Saves all problem states, and associated QValues in the store
	void unserialize(pugi::xml_node* node);	// Loads problem states and associated QValues from given node

protected:
	ProblemStore m_problemStore;
	std::map<int, ProblemState*> m_problemStates;
	World* m_world;
};

#endif // PROBLEM_H
