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

public:
    Perception() {}

    virtual std::vector<PhysicalObject*>* getPercievedObjects() = 0;
    virtual void setPercievedObjects(std::vector<PhysicalObject*> percievedObjects);
};

#endif
