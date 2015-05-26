#include "World.h"


World::World(void)
{
	this->m_map = new Map();
}

World::~World(void)
{
}


void World::createMap()
{
//TODO: clean
	loadLevel();
}

void World::loadLevel(std::string path)
{
	// Clearing previous objects
	this->m_map->clear();
	this->m_bodies.clear();
	this->m_influences.clear();
	this->m_objects.clear();

	this->currentLevelPath = path;
	if (path.compare("Default") == 0)	// Identical
	{
		// Default, hardcoded map
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (i == 1 && j == 1)
				{
					// Lemming start
					createBody(i, j);
				}
				else if (i == 8 && j == 8)
				{
					// Exit
					createObject(i, j, SEMANTIC::T_EXIT);
				}
				else if (i == 0 || i == 9 || j == 0 || j == 9)
				{
					// Rocks
					createObject(i, j, SEMANTIC::T_ROCK);
				}
				else
				{
					// Dirt
					createObject(i, j, SEMANTIC::T_DIRT);
				}
			}
		}
	}

	//TODO: implement level loading from pugixml
}

Body* World::createBody(int x, int y)
{
	// Cell exists and isn't null : can't add
	if (this->m_map->isCellCreated(x, y) && !this->m_map->isCellEmpty(x, y))
	{
		std::cout << "ERROR : World::createBody : can't add on cell " << x << ", " << y << " : cell already occupied" << endl;
		return NULL;
	}

	// Creating the new body
	Body* b = new BodyLemming(SEMANTIC::B_LEMMING);

	// Adding body to map
	if (this->m_map->addWorldObject(x, y, b))
	{
		// Insertion successfull
		b->setPosition(x, y);	// setting intitial position
		b->setMap(m_map->getMap());	// setting current map
		this->m_bodies.push_back(b);	// adding the body the bodies container
		return b;
	}
	else
	{
		// Insertion failed : clean memory and return
		delete(b);
		std::cout << "ERROR : World::createBody : couldn't insert body in the map" << endl;
		return NULL;
	}
}

PhysicalObject* World::createObject(int x, int y, SEMANTIC type)
{
	// Cell exists and isn't null : can't add
	if (this->m_map->isCellCreated(x, y) && !this->m_map->isCellEmpty(x, y))
	{
		std::cout << "ERROR : World::createObject : can't add on cell " << x << ", " << y << " : cell already occupied" << endl;
		return NULL;
	}

	PhysicalObject* o = new Terrain(type);

	// Adding object to map
	if (this->m_map->addWorldObject(x, y, o))
	{
		// Insertion successfull
		o->setPosition(x, y);
		this->m_objects.push_back(o);
		return o;
	}
	else
	{
		// Insertion failed : clean
		delete(o);
		std::cout << "ERROR : World::createObject : couldn't insert object in the map" << endl;
		return NULL;
	}
}

// Completely removes given object from the map and the object container
void World::removeObject(PhysicalObject* object)	
{
	this->m_map->removeWorldObject(object);
	
	for (std::vector<PhysicalObject*>::iterator it = this->m_objects.begin(); it != this->m_objects.end(); ++it)
	{
		if ((*it) == object)
		{
			// Found the object : clean memory, and return
			it = this->m_objects.erase(it);
			return;
		}
	}
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
		//std::cout << "Influences from body " << i << ", at position " << x << "," << y << endl;

        // Below is empty : fall
        if (m_map->isCellCreated(x, y+1) && m_map->getCell(x, y+1)->getWorldObject() == NULL)
        {
			if (this->m_map->moveWorldObject(x, y,
				x, y + 1))
			{
				m_bodies[i]->setPosition(x, y + 1);
				std::cout << "World::resolveInfluences : Body " << i << " fell down" << endl;
			}
			else
			{
				std::cout << "ERROR : World::resolveInfluences : Body " << i << " couldn't fall down" << endl;
			}
        }
        else
        {
            // Else, resolve normally
            switch(*currentAction)
            {
                // Lemming going right
			case ACTIONS::A_RIGHT:
                if (m_map->isCellCreated(x+1,y))
                {
                    Cell* newCell = m_map->getCell(x+1, y);
                    Cell* oldCell = m_map->getCell(x, y);

					if (newCell->getWorldObject() == NULL)
					{
						// newCell is null : move
						if (this->m_map->moveWorldObject(x, y,
							x+1, y))
						{
							m_bodies[i]->setPosition(x + 1, y);
							std::cout << "World::resolveInfluences : Body " << i << " moved right" << endl;
						}
						else
						{
							std::cout << "ERROR : World::resolveInfluences : Body " << i << " couldn't move right" << endl;
						}
					}
                    else if (newCell->getWorldObject() != NULL
                        && isDiggable(newCell->getWorldObject()->getSemantic()))
                    {
						// newCell is non NULL and diggable : remove that, and move

						removeObject(newCell->getWorldObject());	// removing diggable object
						if (this->m_map->moveWorldObject(x, y,
							x+1, y))
						{
							m_bodies[i]->setPosition(x + 1, y);	// moving lemming to new cell
							std::cout << "World::resolveInfluences : Body " << i << " dug right" << endl;
						}
						else
						{
							std::cout << "ERROR : World::resolveInfluences : Body " << i << " couldn't dig right" << endl;
						}
                    }
                }
                break;
			case ACTIONS::A_LEFT:
                if (m_map->isCellCreated(x-1,y))
                {
                    Cell* newCell = m_map->getCell(x-1, y);
                    Cell* oldCell = m_map->getCell(x, y);

					if (newCell->getWorldObject() == NULL)
					{
						// newCell is null : move
						if (this->m_map->moveWorldObject(x, y,
							x-1, y))
						{
							m_bodies[i]->setPosition(x - 1, y);
							std::cout << "World::resolveInfluences : Body " << i << " moved left" << endl;
						}
						else
						{
							std::cout << "ERROR : World::resolveInfluences : Body " << i << " couldn't move left" << endl;
						}
					}
                        
					
                    else if (newCell->getWorldObject() != NULL
                        && isDiggable(newCell->getWorldObject()->getSemantic()))
                    {
						// newCell is non NULL and diggable : remove that, and move
						removeObject(newCell->getWorldObject());	// removing diggable object

						if (this->m_map->moveWorldObject(x, y,
							x - 1, y))
						{
							m_bodies[i]->setPosition(x - 1, y);	// moving lemming to new cell
							std::cout << "World::resolveInfluences : Body " << i << " dug left" << endl;
						}
						else
						{
							std::cout << "ERROR : World::resolveInfluences : Body " << i << " couldn't dig left" << endl;
						}
                    }
                }
                break;
			case ACTIONS::A_DOWN:
                if (m_map->isCellCreated(x,y+1))
                {
                    Cell* newCell = m_map->getCell(x, y+1);
                    Cell* oldCell = m_map->getCell(x, y);

                    // newCell can't be NULL : lemming would have fell down.
                    // newCell is non NULL and diggable : remove that, and move
                    if (newCell->getWorldObject() != NULL
                        && isDiggable(newCell->getWorldObject()->getSemantic()))
                    {
						// newCell is non NULL and diggable : remove that, and move
						removeObject(newCell->getWorldObject());	// removing diggable object

						if (this->m_map->moveWorldObject(x, y,
							x, y+1))
						{
							m_bodies[i]->setPosition(x, y + 1);	// moving lemming to new cell
							std::cout << "World::resolveInfluences : Body " << i << " dug down" << endl;
						}
						else
						{
							std::cout << "ERROR : World::resolveInfluences : Body " << i << " couldn't dig left" << endl;
						}
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

Map* World::getMap()
{
	return this->m_map;
}

void World::update()
{
    this->collectInfluences();
    this->resolveInfluences();
}

void World::setPerceptions()
{
    for (std::vector<Body*>::iterator currentBody = this->m_bodies.begin(); currentBody != this->m_bodies.end(); ++currentBody)
	{
		setBodyPerception(*currentBody);
	}
}

void World::reset()
{
	loadLevel(this->currentLevelPath);
}

//Private functions

bool World::isDiggable(SEMANTIC semantic)
{
    if (semantic == SEMANTIC::T_DIRT)
        return true;
    return false;
}

void World::setBodyPerception(Body* body)
{
    int x,y;
    body->getPosition(x,y);

    Perception* newPerception = new PerceptionCircle();
    Cell* cell;

    std::vector<PhysicalObject*> perceivedObjects;

    //topleft
    for (int i = -1 ; i <= 1; ++i)
    {
        for (int j = -1 ; j <= 1 ; ++j)
        {
            if (i != 0 || j != 0)
            {
				cell = this->m_map->getCell(x + j, y + i);
				if (cell != NULL)
					perceivedObjects.push_back(cell->getWorldObject());
            }
        }
    }
    newPerception->setPerceivedObjects(perceivedObjects);
    body->setPerception(newPerception);
}
