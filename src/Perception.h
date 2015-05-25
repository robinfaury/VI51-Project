#ifndef PERCEPTION_H_
#define PERCEPTION_H_

#include <vector>
#include "PhysicalObject.h"

/**
*   Base class for perception objects
*/

class Perception
{
protected:
    std::vector<PhysicalObject*> percievedObjects;

	int lemmingX;
	int lemmingY;
	int exitX;
	int exitY;

public:
	Perception();
	Perception(std::vector<PhysicalObject*> percievedObjects);

    virtual std::vector<PhysicalObject*>* getPercievedObjects();
    virtual void setPercievedObjects(std::vector<PhysicalObject*> percievedObjects) = 0;
	
	void getLemmingPos(int& x, int& y);
	int getLemmingX();
	int getLemmingY();

	void getExitPos(int& x, int& y);
	int getExitX();
	int getExitY();
};

#endif
