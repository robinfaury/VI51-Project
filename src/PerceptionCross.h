#ifndef PERCEPTIONCROSS_H_
#define PERCEPTIONCROSS_H_

#include "Perception.h"

/**
*   Percieved objects are stored like this :
*
*		0
*   1	X	2
*		3
*
*  with X being the lemming
*/

class PerceptionCross : public Perception
{
private:

public:
	PerceptionCross(int lemmingX, int lemmingY, int exitX, int exitY);
	virtual void setPerceivedObjects(std::vector<PhysicalObject*> perceivedObjects);
};

#endif
