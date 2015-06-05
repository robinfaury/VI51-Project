#ifndef BODY_H_
#define BODY_H_



#include <map>
#include <glm.hpp>

#include "PhysicalObject.h"
#include "Perception.h"
#include "Map.h"

enum ACTIONS
{
    A_NONE,
	A_RIGHT,
	A_LEFT,
	A_DOWN
};

/** \class Body
*   The Body class is the base class for bodies in the environment. All bodies must derive from it.
*   The Body class defines the perception methods, and the influences that the agent can have.
*/
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

    /**
    *   This function returns the percieved objects (set by the World once per frame)
    */
	virtual Perception* getPerception() = 0;

	/**
	*   Function used by the world to set the perceptions once per frame
	*/
	virtual void setPerception(Perception* newPerception);

	/**
	*   Sets the influence that the action wants to have for this frame.
	*/
	void setInfluence(ACTIONS influence);

	/**
	*   Getter for the influence
	*/
	ACTIONS getInfluence();

	void setMap(std::map<std::pair<int, int>, Cell*>* pMap);

	/**
	*  This function allows the agent to clean up before being removed from the world
	*/
	virtual void destroy();

	~Body(void);
};

#endif
