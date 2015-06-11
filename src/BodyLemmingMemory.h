#ifndef BODYLEMMINGMEMORY_H_
#define BODYLEMMINGMEMORY_H_

#include "BodyLemming.h"
#include "Memory.h"

class BodyLemmingMemory : public BodyLemming
{
private:
Memory memory;

public :
	BodyLemmingMemory(SEMANTIC type);

	virtual void serialize(pugi::xml_node * node);

    Memory* getMemory();

    // Redefining from PhysicalObject to add memory
    virtual void setPosition(int x, int y);
};

#endif
