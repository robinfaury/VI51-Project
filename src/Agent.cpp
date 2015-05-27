#include "Agent.h"


Agent::Agent(Body* body) : linked(false)
{
	linkBody(body);
}

Agent::~Agent(void)
{
}

bool Agent::linkBody(Body* body)
{
	unlinkBody();

	if (body != NULL)
	{
		// Connect to new body
		this->body = body;
		this->linked = true;
		return true;
	}
	return false;
}

bool Agent::unlinkBody()
{
	// If body is currently linked, unlink it
	if (this->body != NULL)
		this->body = NULL;

	// Ensure linked flag is correctly set to false
	this->linked = false;
	return true;
}

// Returns true if agent is currently linked to a body. Else, returns false.
bool Agent::isLinked()
{
	return this->linked;
}