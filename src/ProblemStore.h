#ifndef PROBLEMSTORE_H
#define PROBLEMSTORE_H

#include "pugi\pugixml.hpp"

/*
 * The ProblemStore is the database for storing ProblemStates. For each ProblemState, the ProblemStore will store the QValues that are
 * associated with this State. The ProblemStore contains :
 * - A list of ProblemStates
 * - For each ProblemState, a list of QValues : 1 QValue for each possible action of the ProblemState
 */

#include <iostream>
#include "ProblemState.h"
#include <map>

class ProblemStore
{
public:
    ProblemStore();
    ~ProblemStore();
	void clearProblemStore();

	bool initQValues(std::map<int, ProblemState*> problemStates);

	std::vector<std::string>* getPossibleActions(int stateId); // get all possible actions from the given state
	std::vector<std::string>* getPossibleActions(ProblemState* problemState);

	std::string getBestAction(int stateId);  // Get best action based on the qvalues of the given state
	std::string getBestAction(ProblemState* problemState);  // Override

    float getQValue(int stateId, std::string action);    // For this state and action, return the associated QValue
	float getQValue(ProblemState* problemState, std::string action);    // Override

    void updateQValue(int stateId, std::string action, float newQValue);
	void updateQValue(ProblemState* problemState, std::string action, float newQValue);

	std::string getQValuesReport();
	
	static std::string getOneOf(std::vector<std::string>* possibleActions);

	// Load / save
	void serialize(pugi::xml_node* node);
	void unserialize(pugi::xml_node* node);

    //Getters / Setters
protected:
	// map< stateId, 
	std::map <int, std::map<std::string, float> > m_QValues; // array indexed by ProblemState ids containing arrays of q-values indexed by actions

};

#endif // PROBLEMSTORE_H
