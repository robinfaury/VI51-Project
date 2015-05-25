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
    PerceptionCircle();
	PerceptionCircle(std::vector<PhysicalObject*> percievedObjects);

	virtual void setPercievedObjects(std::vector<PhysicalObject*> percievedObjects);
};

#endif
