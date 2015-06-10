#ifndef AGENTQLEARNING_H_
#define AGENTQLEARNING_H_

#include "Agent.h"
#include "Problem.h"
#include "QValues.h"

/** \class AgentQLearning
*   The AgentQLearning inherits from the Agent class.
*/
class AgentQLearning : public Agent
{
	public:
		AgentQLearning();
		AgentQLearning(Problem* problem);
		~AgentQLearning();

		/** The function containing the decision taking. This function is called once per frame.
		*
		*/
		void live();

	private:
		// Problem with Qvalues learned using the QLearning algorithm
		Problem* m_problem;
};

#endif
