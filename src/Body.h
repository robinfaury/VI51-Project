#ifndef BODY_H_
#define BODY_H_

#include <map>
#include <glm.hpp>

#include "PhysicalObject.h"
#include "Perception.h"
#include "Map.h"

enum ACTIONS
{
    NONE,
    RIGHT,
    LEFT,
    DOWN
};

class Body:  public PhysicalObject
{
protected:
	int m_x;
	int m_y;
	ACTIONS m_influence;
	std::map<std::pair<int,int>, Cell*>* m_map;
	Perception* m_perception;

public:
	Body(SEMANTIC type);

	virtual Perception* getPerception() = 0;
	virtual void setPerception(Perception* newPerception);
	void setInfluence(ACTIONS influence);
	ACTIONS getInfluence();

	void setMap(std::map<std::pair<int, int>, Cell*>* pMap);

	virtual void destroy();
	~Body(void);
};

#endif
