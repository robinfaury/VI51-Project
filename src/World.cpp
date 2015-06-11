#include "World.h"


World::World(std::string* currentLevelPath) : currentMap(currentLevelPath), exitX(-1), exitY(-1), size(DEFAULT_SIZE)
{
    this->m_map = new Map();
	this->mapGenerator = NULL;
}

World::~World(void)
{
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
    levelNode.append_attribute("size").set_value(this->size);

    // saving tiles
    cout << "Map : serializeMap : Cells" << endl;
    pugi::xml_node cells = levelNode.append_child("Cells");
    pugi::xml_node tempCell;
    pugi::xml_node tempCellObject;

    // Iterating on every cell
    for (std::map<std::pair<int, int>, Cell*>::iterator it = this->getMap()->getMap()->begin(); it != this->getMap()->getMap()->end(); ++it)
    {
        tempCell = cells.append_child("Cell");

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
	std::string completePath = sourcesPath;
	completePath = completePath + resPath + mapPath + path + extensionPath;
	//cout << "Saving result : " << completePath.data() << " : " << doc.save_file("test") << endl;
    cout << "Saving result : " << completePath.data() << " : " << doc.save_file(completePath.data()) << endl;
}

bool World::loadLevel(std::string path)
{
	// Clearing previous objects
	this->m_map->clear();
	this->m_bodies.clear();
	this->m_influences.clear();
	this->m_objects.clear();

	std::string completePath = sourcesPath;
	completePath = completePath + resPath + mapPath + path + extensionPath;

	if (path.compare("Generate") == 0)	// Identical
	{
		std::cout << "World::Generating new map with size " << this->size << std::endl;
		this->generateLevel();
	}
	else
	{
		// Clearing previous level
		this->m_bodies.clear();
		this->m_objects.clear();

		// Loading doc
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(completePath.data());
		if (result.status != pugi::xml_parse_status::status_ok)
		{
			// Error occured
			std::cout << "ERROR : World::LoadLevel : unable to parse given file : " << completePath << std::endl;
			std::cout << "Aborting map loading..." << std::endl;
			return false;
		}
		//cout << "Load result: " << result.description() << endl;

		pugi::xml_node levelNode = doc.child("Level");
		this->size = levelNode.attribute("size").as_int();
		pugi::xml_node cells = levelNode.child("Cells");

		pugi::xml_node tempCell = cells.first_child();
		pugi::xml_node tempChild;
		while (tempCell)
		{
			if (tempCell.attribute("hasChild").as_bool())
			{
				tempChild = tempCell.child("object");
				this->deserializeObject(&tempChild);
			}
			else
			{
				this->m_map->createCell(static_cast<SEMANTIC>(tempCell.attribute("x").as_int()), static_cast<SEMANTIC>(tempCell.attribute("y").as_int()), NULL);
			}
			tempCell = tempCell.next_sibling();
		}
	}
	findExit(this->exitX, this->exitY);
	setPerceptions();
	return true;
}

void World::resetMap()	// Resets map with current level path
{
	loadLevel(*this->currentMap);
}

void World::generateLevel()
{
	// Clearing previous objects
	this->m_map->clear();
	this->m_bodies.clear();
	this->m_influences.clear();
	this->m_objects.clear();


	this->mapGenerator = new MapGenerator(this->size);
	this->mapGenerator->setWorld(this);
	this->mapGenerator->generateWithAutoSeeds();

	delete this->mapGenerator;
	this->mapGenerator = NULL;
}

// finds the exit and sets its position to the two given integers
void World::findExit(int& exitX, int& exitY)
{
	for (std::vector<PhysicalObject*>::iterator it = this->m_objects.begin(); it != this->m_objects.end(); ++it)
	{
		if ((*it)->getSemantic() == SEMANTIC::T_EXIT)
		{
			exitX = (*it)->getPosition().at(0);
			exitY = (*it)->getPosition().at(1);
			return;
		}
	}
}

// Checks if at least one lemming managed to get to the exit
bool World::lemmingSuccess()
{
	int x, y, lemmingX, lemmingY;
	findExit(x, y);

	for (std::vector<Body*>::iterator it = this->m_bodies.begin(); it != this->m_bodies.end(); ++it)
	{
		(*it)->getPosition(lemmingX, lemmingY);
		if (lemmingX == x && lemmingY == y)
			return true;
	}
	return false;
}

int World::getSize()
{
	return this->size;
}

// Sets the size of the map (in number of cells square)
void World::setSize(int size)
{
	if (size < DEFAULT_SIZE)
		size = DEFAULT_SIZE;

	this->size = size;
}

Body* World::createBody(int x, int y)
{
    // Cell exists and isn't null : can't add
    if (this->m_map->isCellCreated(x, y) && !this->m_map->isCellEmpty(x, y))
    {
        std::cout << "ERROR : World::createBody : can't add on cell " << x << ", " << y << " : cell already occupied" << endl;
        return NULL;
    }

    Body* b = new BodyLemmingMemory(SEMANTIC::B_LEMMING);

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
			if (WORLD_DEBUG)
				std::cout << "World::removeObject : Found and removed an object at " << (*it)->getPosition().at(0) << "," << (*it)->getPosition().at(1) << std::endl;

			it = this->m_objects.erase(it);
            return;
        }
    }
	if (WORLD_DEBUG)
		std::cout << "World::removeObject : Didn't find " << std::endl;
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
				if (WORLD_DEBUG)
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
							if (WORLD_DEBUG)
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
							if (WORLD_DEBUG)
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
							if (WORLD_DEBUG)
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
							if (WORLD_DEBUG)
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
							if (WORLD_DEBUG)
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
	this->setPerceptions();
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
    Perception* newPerception = new PerceptionCircle(x,y,this->exitX, this->exitY);
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


bool World::checkValidPosition(int tileX, int tileY)
{
	Cell* cell = this->m_map->getCell(tileX, tileY);
	if (cell == NULL)
		return false;

	PhysicalObject* object = cell->getWorldObject();
	if (object == NULL)
		return true;

	switch (object->getSemantic())
	{
	case SEMANTIC::B_LEMMING :
	case SEMANTIC::T_DIRT :
	case SEMANTIC::T_EXIT :
		return true;
	default :
		return false;
	}
}

bool World::forceLemmingPosition(int tileX, int tileY)
{
	if (this->checkValidPosition(tileX, tileY))
	{
		// Lemming cleaned from the map
		this->m_map->removeWorldObject(this->m_bodies.at(0));
		Cell* cell = this->m_map->getCell(tileX, tileY);
		PhysicalObject* object = cell->getWorldObject();
		if (object != NULL)
			this->removeObject(object);

		cell->setWorldObject(this->m_bodies.at(0));
		this->m_bodies.at(0)->setPosition(tileX, tileY);

		return true;
	}
	return false;
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


//Private functions

bool World::isDiggable(SEMANTIC semantic)
{
    if (semantic == SEMANTIC::T_DIRT || semantic == SEMANTIC::T_EXIT)
        return true;
    return false;
}

void World::setBodyPerception(Body* body)
{
    int x,y;
    body->getPosition(x,y);

    Perception* newPerception = new PerceptionCircle(x, y, this->exitX, this->exitY);
    Cell* cell;

    std::vector<PhysicalObject*> perceivedObjects;

	int xOffset, yOffset;
	xOffset = 0;
	yOffset = 0;
	for (int i = 0; i < 8; ++i)
	{
		switch (i)
		{
		case 0:
			xOffset = -1;
			yOffset = -1;
			break;
		case 1:
			xOffset = 0;
			yOffset = -1;
			break;
		case 2:
			xOffset = 1;
			yOffset = -1;
			break;
		case 3:
			xOffset = -1;
			yOffset = 0;
			break;
		case 4:
			xOffset = +1;
			yOffset = 0;
			break;
		case 5:
			xOffset = -1;
			yOffset = 1;
			break;
		case 6:
			xOffset = 0;
			yOffset = 1;
			break;
		case 7:
			xOffset = 1;
			yOffset = 1;
			break;
		}

		cell = this->m_map->getCell(x + xOffset, y + yOffset);
		if (cell != NULL)
		{
			perceivedObjects.push_back(cell->getWorldObject());
		}
		else
		{
			std::cout << "ERROR : World::setBodyPerception : fatal error, couldn't find cell at pos " << x + xOffset << "," << y + yOffset << std::endl;
		}
	}

    newPerception->setPerceivedObjects(perceivedObjects);
    body->setPerception(newPerception);
}


PhysicalObject* World::deserializeObject(pugi::xml_node* objectNode)
{
	PhysicalObject* ret = NULL;
	switch (static_cast<SEMANTIC>(objectNode->attribute("semantic").as_int()))
	{
	case SEMANTIC::B_LEMMING :
		ret = createBody(static_cast<SEMANTIC>(objectNode->attribute("x").as_int()), static_cast<SEMANTIC>(objectNode->attribute("y").as_int()));
		break;
	default :
		ret = createObject(static_cast<SEMANTIC>(objectNode->attribute("x").as_int()),
			static_cast<SEMANTIC>(objectNode->attribute("y").as_int()),
			static_cast<SEMANTIC>(objectNode->attribute("semantic").as_int()));
	}
	return ret;
}
