#ifndef PERCEPTIONCIRCLE_H_
#define PERCEPTIONCIRCLE_H_

#include "Perception.h"

/**
*   Percieved objects are stored like this :
*
*   0   1   2
*   3   X   4
*   5   6   7
*
*  with X being the lemming
*/

class PerceptionCircle : public Perception
{
private:

public:
	PerceptionCircle(int lemmingX, int lemmingY, int exitX, int exitY);

	virtual void setPerceivedObjects(std::vector<PhysicalObject*> perceivedObjects);
};

#endif
