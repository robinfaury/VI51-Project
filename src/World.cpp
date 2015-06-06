#include "World.h"


World::World(void)
{
    this->m_map = new Map();
	this->mapGenerator == NULL;
}

World::~World(void)
{
}


void World::createMap()
{
//TODO: clean
	generateLevel();
}

// Loading/saving level
//TODO: save level
/**
*   This function saves the level in the given filepath, under xml format
*/
void World::saveLevel(std::string path)
{
    pugi::xml_document doc;
    pugi::xml_node levelNode = doc.append_child("Level");

    //Serializing map
    cout << "Map : serializeMap : begin" << endl;

    cout << "Map : serializeMap : mapInfo" << endl;
    levelNode.append_attribute("mapSize").set_value((int)this->getMap()->getMap()->size());

    // saving tiles
    cout << "Map : serializeMap : Cells" << endl;
    pugi::xml_node cells = levelNode.append_child("Cells");
    pugi::xml_node tempCell;
    pugi::xml_node tempCellObject;

    // Iterating on every cell
    int i = 0;
    for (std::map<std::pair<int, int>, Cell*>::iterator it = this->getMap()->getMap()->begin(); it != this->getMap()->getMap()->end(); ++it)
    {
        tempCell = cells.append_child("Cell" + i);
        ++i;

        // Setting cell coordinate
        tempCell.append_attribute("x").set_value(it->first.first);
        tempCell.append_attribute("y").set_value(it->first.second);

        if (it->second->getWorldObject() == NULL)
            tempCell.append_attribute("hasChild").set_value(false);
        else
        {
            tempCell.append_attribute("hasChild").set_value(true);
            tempCellObject = tempCell.append_child("object");
            it->second->getWorldObject()->serialize(&tempCellObject);
        }

    }
    cout << "Map : serializeMap : Done" << endl;
    std::string completePath = "Maps/" + path;
    cout << "Saving result : " << completePath.data() << doc.save_file(completePath.data()) << endl;
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
		for (int i = 0; i < 10; ++i) //largeur
		{
			for (int j = 0; j < 10; ++j) //hauteur
			{
				if (i == 4 && j == 4)
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
	else
	{

	}
}

void World::generateLevel()
{
	// Clearing previous objects
	this->m_map->clear();
	this->m_bodies.clear();
	this->m_influences.clear();
	this->m_objects.clear();

	this->mapGenerator = new MapGenerator();
	this->mapGenerator->setWorld(this);
	this->mapGenerator->generate(40, 15);

	delete this->mapGenerator;
	this->mapGenerator == NULL;
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
		//std::cout << "ERROR : World::createObject : can't add on cell " << x << ", " << y << " : cell already occupied" << endl;
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

// Completely removes object at given position from the map and the object container
void World::removeObject(int x, int y)
{
    if (this->m_map->isCellCreated(x, y))
    {
        Cell* cell = this->m_map->getCell(x, y);
        PhysicalObject* object = cell->getWorldObject();
        if (object != NULL)
            removeObject(object);
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

/**
*   Called by the LearningMethods. This function returns what would a body percieve if it was standing at this position
*/
Perception* World::getPerceptionFromTile(int x, int y)
{
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
    return newPerception;
}

/**
*   Called by the LearningMethods. This function returns all possible lemming perceptions (ignoring above objects)
*
*   Perceptions are stored like this :
*   ?   ?   ?
*   [0] X   [1]
*   [2] [3] [4]
*
*   where ? is unknown (and not contained in the vector), and X is the lemming
*/
std::vector<std::vector<SEMANTIC>> getAllPossiblePerceptions()
{
    std::vector<std::vector<SEMANTIC>> ret;
    std::vector<SEMANTIC> state;
    for (int i = 0; i < SEMANTIC_SIZE ; ++i)
    {
        if (i == (int)SEMANTIC::T_EXIT)
            continue;
        for (int j = 0; j < SEMANTIC_SIZE; ++j)
        {
            for (int k = 0; k < SEMANTIC_SIZE; ++k)
            {
                if (k == (int)SEMANTIC::T_EXIT)
                    continue;
                for (int l = 0; l < SEMANTIC_SIZE ; ++l)
                {
                    for (int m = 0; m < SEMANTIC_SIZE ; ++m)
                    {
                        state.clear();
                        state.push_back((SEMANTIC)i);
                        state.push_back((SEMANTIC)j);
                        state.push_back((SEMANTIC)k);
                        state.push_back((SEMANTIC)l);
                        state.push_back((SEMANTIC)m);
                        ret.push_back(state);
                    }
                }
            }
        }
    }
    return ret;
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
