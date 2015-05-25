#include "GraphicView.h"


GraphicView::GraphicView(void)
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
}

int GraphicView::checkEvent()
{
	sf::Event event;
    while (this->window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
		{
            this->window->close();
			return 1;
		}
    }
	return 0;
}

void GraphicView::draw()
{
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
                this->terrainSprite.setPosition(x*TILE_SIZE, y*TILE_SIZE);
                window->draw(terrainSprite);
            }
            else
            {
                // Else, draw the lemming
                this->lemmingSprite.setPosition(x*TILE_SIZE, y*TILE_SIZE);
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

//Private functions

bool GraphicView::setTextureRectFromSemantic(SEMANTIC semantic)
{
    // If its a type of terrain, set textureRect, then return true
	switch (semantic)
	{
	case SEMANTIC::T_DIRT :
		terrainSprite.setTextureRect(sf::IntRect(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		break;
	case SEMANTIC::T_EXIT :
		terrainSprite.setTextureRect(sf::IntRect(2*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		break;
	case SEMANTIC::T_ROCK :
		terrainSprite.setTextureRect(sf::IntRect(0*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		break;
	default :
		// Lemming
		return false;
	}
	return true;
}
