#include "GraphicView.h"


GraphicView::GraphicView(void) : initialized(false)
{
}

void GraphicView::init(int height, int width, std::map<std::pair<int, int>, Cell*>* map)
{
	this->currentMap = map;
	this->window = new sf::RenderWindow(sf::VideoMode(height, width), "VI51 - Lemmings");
	this->window->setVerticalSyncEnabled(true);

	// Textures loading
	if (!lemmingTexture.loadFromFile(LEMMINGTEX_PATH))
	{
        std::cout << "ERROR : couldn't load lemming texture from " << LEMMINGTEX_PATH << endl;
	}
	lemmingSprite.setTexture(lemmingTexture);

	if (!terrainTexture.loadFromFile(TERRAINTEX_PATH))
	{
        std::cout << "ERROR : couldn't load terrain texture from " << TERRAINTEX_PATH << endl;
	}
	terrainSprite.setTexture(terrainTexture);

	std::cout << "Graphics initialised" << endl;

	initialized = true;
}

void GraphicView::clear()
{
    if (initialized)
    {
        this->window->close();
        initialized = false;
    }
}

sf::RenderWindow* GraphicView::getWindow()
{
	return this->window;
}

void GraphicView::draw()
{
    if (!initialized)
        return;

	window->clear(sf::Color::Black);

	int x, y;
	// For each cell of the map
	for (std::map<std::pair<int, int>, Cell*>::iterator it = currentMap->begin(); it != currentMap->end(); ++it)
	{
        PhysicalObject* object = it->second->getWorldObject();
        // If world object isn't NULL, draw it
        if (object != NULL)
        {
            x = it->first.first;
            y = it->first.second;
            // If it's a terrain, draw it with mapSprite
            if (setTextureRectFromSemantic(object->getSemantic()))
            {
                this->terrainSprite.setPosition(static_cast<float>(x*TILE_SIZE), static_cast<float>(y*TILE_SIZE));
                window->draw(terrainSprite);
            }
            else
            {
                // Else, draw the lemming
                this->lemmingSprite.setPosition(static_cast<float>(x*TILE_SIZE), static_cast<float>(y*TILE_SIZE));
                window->draw(lemmingSprite);
            }
        }
	}
	window->display();
}

void GraphicView::setWorld(World* world)
{
	this->world = world;
}

GraphicView::~GraphicView(void)
{
	this->window->clear();
	delete this->window;
}

void GraphicView::setUserAction(USER_ACTIONS action)
{
	this->userAction = action;
}

USER_ACTIONS GraphicView::getUserAction()
{
	return this->userAction;
}

void GraphicView::convertCoordinates_worldToTiles(int px, int py, int& wx, int& wy)
{
	wx = px / TILE_SIZE;
	wy = py / TILE_SIZE;
}

//Private functions

bool GraphicView::setTextureRectFromSemantic(SEMANTIC semantic)
{
    // If its a type of terrain, set textureRect, then return true
	switch (semantic)
	{
	case SEMANTIC::T_BOUND:
		terrainSprite.setTextureRect(sf::IntRect(0 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		break;
	case SEMANTIC::T_DIRT :
		terrainSprite.setTextureRect(sf::IntRect(1 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		break;
	case SEMANTIC::T_EXIT :
		terrainSprite.setTextureRect(sf::IntRect(2 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		break;
	case SEMANTIC::T_ROCK:
		terrainSprite.setTextureRect(sf::IntRect(3 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		break;
	default :
		// Lemming
		return false;
	}
	return true;
}

