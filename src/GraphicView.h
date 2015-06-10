#ifndef GRAPHICVIEW_H_
#define GRAPHICVIEW_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define TILE_SIZE 32
#define LEMMINGTEX_PATH "res/lemming.png"
#define TERRAINTEX_PATH "res/terrain.png"
#define ICONNNTEX_PATH "res/icons/iconNN.png"
#define ICONQLTEX_PATH "res/icons/iconQL.png"

#include "World.h"

enum USER_ACTIONS
{
	U_CLEAR,
	U_DIRT,
	U_ROCK,
};

/** \class GraphicView
*   The GraphicView is the graphics interface. It draws the world everyframe.
*/
class GraphicView
{
private:
	sf::RenderWindow* window;
	World* world;

	float tileSizeX;
	float tileSizeY;

	// World textures
	sf::Texture terrainTexture;
	sf::Texture lemmingTexture;

	//Icons
	sf::Texture image_iconNN;
	sf::Texture image_iconQL;

	sf::Sprite iconNN;
	sf::Sprite iconQL;

	sf::Sprite terrainSprite;
	sf::Sprite lemmingSprite;


	std::map<std::pair<int, int>, Cell*>* currentMap;

	bool setTextureRectFromSemantic(SEMANTIC semantic);

	USER_ACTIONS userAction;

	bool initialized;

public:
	GraphicView(void);

    /**
    *   Initialises the view
    */
	void init(int height, int width, std::map<std::pair<int, int>, Cell*>* map);
	void clear();
	sf::RenderWindow* getWindow();

	/**
	*   Draw the world
	*/
	void draw();

	void setWorld(World* world);

	~GraphicView(void);

	// User interface stuff
	void setUserAction(USER_ACTIONS action);
	USER_ACTIONS getUserAction();

	void convertCoordinates_worldToTiles(int px, int py, int& wx, int& wy);
};

#endif
