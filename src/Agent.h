#ifndef AGENT_H_
#define AGENT_H_


#include "Body.h"


/** \class Agent
*   The Agent class is the base class for implementing agent behaviours. It defines the functions used by the simulator at runtime.
*   An agent needs to have a linked body. If it isn't linked to a body, it won't be doing anything.
*/
class Agent
{
protected:
	Body* body;     //! The body this agent is linked to
	bool linked;	//! Flag to indicate if the agent is currently linked

public:
	Agent(Body* body = NULL);

    /** The function containing the decision taking. This function is called once per frame.
    *
    */
	virtual void live() = 0;

	// Body connection stuff

	/**
	*   Connects the agent to the given body
	*/
	virtual bool linkBody(Body* body);	// Connects agent to given body. Return true if connection successful

	/**
	*   Disconnects the agent from it's body
	*/
	virtual bool unlinkBody();	// Disconnects agent from body (if it has one). Returns true if disconnection successful.

	/**
	*   Returns true if the agent is currently linked
	*/
	virtual bool isLinked();	// Returns true if agent is currently linked to a body. Else, returns false.

	~Agent(void);
};

#endif
