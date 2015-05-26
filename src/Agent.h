#ifndef AGENT_H_
#define AGENT_H_

#include "Body.h"

class Agent
{
protected:
	Body* body;
	bool linked;	// True if agent is connected to a body.

public:
	Agent(Body* body = NULL);

	virtual void live() = 0;

	// Body connection stuff
	virtual bool linkBody(Body* body);	// Connects agent to given body. Return true if connection successful
	virtual bool unlinkBody();	// Disconnects agent from body (if it has one). Returns true if disconnection successful.
	virtual bool isLinked();	// Returns true if agent is currently linked to a body. Else, returns false.

	~Agent(void);
};

#endif
