#ifndef PERCEPTION_H_
#define PERCEPTION_H_

#include <vector>
#include <iostream>
#include "PhysicalObject.h"

/**
*   Base class for perception objects
*/

class Perception
{
protected:
    std::vector<PhysicalObject*> perceivedObjects;

	int lemmingX;
	int lemmingY;
	int exitX;
	int exitY;

public:
	Perception(int lemmingX, int lemmingY, int exitX, int exitY);
	Perception(std::vector<PhysicalObject*> perceivedObjects);

	virtual void display();

    virtual std::vector<PhysicalObject*>* getPerceivedObjects();
    virtual void setPerceivedObjects(std::vector<PhysicalObject*> perceivedObjects) = 0;
	
	void getLemmingPos(int& x, int& y);
	int getLemmingX();
	int getLemmingY();

	void getExitPos(int& x, int& y);
	int getExitX();
	int getExitY();
};

#endif
