#include "AgentLemmingDummy.h"

AgentLemmingDummy::AgentLemmingDummy(Body* body) : Agent(body), dumbizator(false)
{
}

void AgentLemmingDummy::live()
{
    if(this->body == NULL)
        return;

    if (dumbizator)
    {
        dumbizator = false;
        this->body->setInfluence(ACTIONS::A_RIGHT);
    }
    else
    {
        dumbizator = true;
        this->body->setInfluence(ACTIONS::A_DOWN);
    }
}
