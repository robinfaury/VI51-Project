#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "PhysicalObject.h"

class Terrain : public PhysicalObject
{
private:

public:
	Terrain(SEMANTIC type);

	~Terrain(void);

	virtual void destroy();
};

#endif
