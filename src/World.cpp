#include "World.h"


World::World(void)
{
}

World::~World(void)
{
}


void World::createMap()
{
//TODO: clean
	loadLevel();

	// For each body
	for (std::vector<Body*>::iterator currentBody = this->m_bodies.begin(); currentBody != this->m_bodies.end(); ++currentBody)
		(*currentBody)->setMap(this->m_map->getMap());
}

void World::loadLevel()
{
// TODO: clean

}

Body* World::createBody(int x, int y)
{
	Body* b = new BodyLemming(new B_Lemming());
	b->setPosition(x, y);
	b->setMap(m_map->getMap());
	this->m_bodies.push_back(b);
	return b;
}


void World::collectInfluences()
{
	this->m_influences.clear();
	for (std::vector<Body*>::iterator currentBody = this->m_bodies.begin(); currentBody != this->m_bodies.end(); ++currentBody)
	{
		this->m_influences.push_back((*currentBody)->getInfluence());
	}
}

void World::resolveInfluences()
{
    int i = 0;
    for (std::vector<ACTIONS>::iterator currentAction = m_influences.begin() ;
        currentAction != m_influences.end() ;
        ++currentAction)
    {
        int x,y;
        m_bodies[i]->getPosition(x, y);

        // Below is empty : fall
        if (m_map->isCellCreated(x, y+1) && m_map->getCell(x, y+1)->getWorldObject() == NULL)
        {
            Cell* newCell = m_map->getCell(x, y+1);
            Cell* oldCell = m_map->getCell(x, y);

            newCell->setWorldObject(oldCell->getWorldObject());
            oldCell->setWorldObject(NULL);
            m_bodies[i]->setPosition(x,y+1);
        }
        else
        {
            // Else, resolve normally
            switch(*currentAction)
            {
                // Lemming going right
                case RIGHT:
                    if (m_map->isCellCreated(x+1,y))
                    {
                        Cell* newCell = m_map->getCell(x+1, y);
                        Cell* oldCell = m_map->getCell(x, y);

                        // newCell is null : move
                        if (newCell->getWorldObject() == NULL)
                            m_bodies[i]->setPosition(x+1, y);
                        // newCell is non NULL and diggable : remove that, and move
                        else if (newCell->getWorldObject() != NULL
                            && isDiggable(newCell->getWorldObject()->getSemantic()))
                        {
                            m_bodies[i]->setPosition(x+1, y);
                        }
                    }
                    break;
                case LEFT:
                    if (m_map->isCellCreated(x-1,y))
                    {
                        Cell* newCell = m_map->getCell(x-1, y);
                        Cell* oldCell = m_map->getCell(x, y);

                        // newCell is null : move
                        if (newCell->getWorldObject() == NULL)
                            m_bodies[i]->setPosition(x-1, y);
                        // newCell is non NULL and diggable : remove that, and move
                        else if (newCell->getWorldObject() != NULL
                            && isDiggable(newCell->getWorldObject()->getSemantic()))
                        {
                            m_bodies[i]->setPosition(x-1, y);
                        }
                    }
                    break;
                case ACTIONS::DOWN :
                    if (m_map->isCellCreated(x,y+1))
                    {
                        Cell* newCell = m_map->getCell(x, y+1);
                        Cell* oldCell = m_map->getCell(x, y);

                        // newCell is null : move
                        if (newCell->getWorldObject() == NULL)
                            m_bodies[i]->setPosition(x, y+1);
                        // newCell is non NULL and diggable : remove that, and move
                        else if (newCell->getWorldObject() != NULL
                            && isDiggable(newCell->getWorldObject()->getSemantic()))
                        {
                            m_bodies[i]->setPosition(x, y+1);
                        }
                    }
                    break;
            }
        }
        ++i;
    }
}

std::vector<Body*>* World::getBodies()
{
	return &this->m_bodies;
}

void World::update()
{
    this->collectInfluences();
    this->resolveInfluences();
    this->setPerceptions();
}

void World::setPerceptions()
{
    for (std::vector<Body*>::iterator currentBody = this->m_bodies.begin(); currentBody != this->m_bodies.end(); ++currentBody)
	{
		setBodyPerception(*currentBody);
	}
}

//Private functions

bool World::isDiggable(Semantic* semantic)
{
    if (typeid(*semantic) == typeid(T_Dirt))
        return true;
    return false;
}

void World::setBodyPerception(Body* body)
{
    int x,y;
    body->getPosition(x,y);

    Perception* newPerception = new PerceptionCircle();
    Cell* cell;

    std::vector<PhysicalObject*> percievedObjects;

    //topleft
    for (int i = -1 ; i <= 1; ++i)
    {
        for (int j = -1 ; j <= 1 ; ++j)
        {
            if (i == 0 && j == 0)
            {

            }
            else
            {
                cell = this->m_map->getCell(x+j,y+i);
                if (cell != NULL)
                    percievedObjects.push_back(cell->getWorldObject());
            }
        }
    }
    newPerception->setPercievedObjects(percievedObjects);
    body->setPerception(newPerception);
}
