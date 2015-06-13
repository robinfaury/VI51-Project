#include "GraphicView.h"


GraphicView::GraphicView(void) : initialized(false), currentIcon(ICON_NONE)
{
}

void GraphicView::init(int height, int width, std::map<std::pair<int, int>, Cell*>* map, bool* lemmingVictory)
{
	this->lemmingVictory = lemmingVictory;

	this->currentMap = map;
	this->window = new sf::RenderWindow(sf::VideoMode(height, width), "VI51 - Lemmings");
	this->window->setVerticalSyncEnabled(true);

	// Textures loading
	std::string lemmingTexturePath = sourcesPath;
	lemmingTexturePath += LEMMINGTEX_PATH;
	if (!lemmingTexture.loadFromFile(lemmingTexturePath))
	{
		std::cout << "ERROR : couldn't load lemming texture from " << LEMMINGTEX_PATH << endl;
	}
	lemmingSprite.setTexture(lemmingTexture);

	std::string terrainTexturePath = sourcesPath;
	terrainTexturePath += TERRAINTEX_PATH;
	if (!terrainTexture.loadFromFile(terrainTexturePath))
	{
		std::cout << "ERROR : couldn't load terrain texture from " << TERRAINTEX_PATH << endl;
	}
	terrainSprite.setTexture(terrainTexture);

	// Icon images
	std::string iconNnTexturePath = sourcesPath;
	iconNnTexturePath += "res/icons/icon_NN.png";
	if (!this->image_iconNN.loadFromFile(iconNnTexturePath))
	{
		std::cout << "ERROR : couldn't load iconNN texture from " << iconNnTexturePath << endl;
	}
	iconNN.setTexture(image_iconNN);

	std::string iconQlTexturePath = sourcesPath;
	iconQlTexturePath += "res/icons/icon_QL.png";
	if (!this->image_iconQL.loadFromFile(iconQlTexturePath))
	{
		std::cout << "ERROR : couldn't load iconNN texture from " << iconQlTexturePath << endl;
	}
	iconQL.setTexture(image_iconQL);

	//Victory image
	std::string victoryTexturePath = sourcesPath;
	victoryTexturePath += "res/icons/victory.png";
	if (!this->victoryTexture.loadFromFile(victoryTexturePath))
	{
		std::cout << "ERROR : couldn't load victory texture from " << victoryTexturePath << endl;
	}
	victorySprite.setTexture(victoryTexture);
	
	this->iconNN.setScale(1.0f, 1.0f);
	this->iconNN.setScale(0.5f, 0.5f);
	this->iconNN.setPosition(width - 64, 0.0f);

	this->iconQL.setScale(1.0f, 1.0f);
	this->iconQL.setScale(0.5f, 0.5f);
	this->iconQL.setPosition(width - 64, 0.0f);

	
	this->victorySprite.setScale(0.25f, 0.25f);
	this->victorySprite.setPosition((float)width / 2 - 64, (float)height / 2 - 64);

	resize(height, width);

	std::cout << "Graphics initialised" << endl;

	initialized = true;
}

void GraphicView::resize(int width, int height)
{
	this->lemmingSprite.setScale(1.0f, 1.0f);
	this->terrainSprite.setScale(1.0f, 1.0f);
	this->tileSizeX = static_cast<float>(this->window->getSize().x) / (float)this->world->getSize();
	this->tileSizeY = static_cast<float>(this->window->getSize().y) / (float)this->world->getSize();
	sf::Vector2f scale(this->tileSizeX / TILE_SIZE, this->tileSizeY / TILE_SIZE);
	this->lemmingSprite.scale(scale);
	this->terrainSprite.scale(scale);

	
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
                this->terrainSprite.setPosition(static_cast<float>(x*this->tileSizeX), static_cast<float>(y*this->tileSizeY));
                window->draw(terrainSprite);
            }
            else
            {
                // Else, draw the lemming
                this->lemmingSprite.setPosition(static_cast<float>(x*this->tileSizeX), static_cast<float>(y*this->tileSizeY));
                window->draw(lemmingSprite);
            }
        }
	}

	// AI icon
	switch (this->currentIcon)
	{
	case DISPLAY_ICON::ICON_NEURALNETWORK :
		window->draw(this->iconNN);
		break;
	case DISPLAY_ICON::ICON_QLEARNING:
		window->draw(this->iconQL);
		break;
	}

	//Victory icon
	if ((*this->lemmingVictory) == true)
		window->draw(victorySprite);

	window->display();
}

void GraphicView::setIconDisplay(DISPLAY_ICON icon)
{
	this->currentIcon = icon;
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
	wx = (int)(px / tileSizeX);
	wy = (int)(py / tileSizeY);
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

