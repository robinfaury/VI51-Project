#ifndef GRAPHICVIEW_H_
#define GRAPHICVIEW_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define TILE_SIZE 32
#define LEMMINGTEX_PATH "res/lemming.png"
#define TERRAINTEX_PATH "res/terrain.png"

#include "World.h"

enum USER_ACTIONS
{
	U_CLEAR,
	U_DIRT,
	U_ROCK,
	U_EXIT,
	U_BOUND,
	U_NONE
};

enum DISPLAY_ICON
{
	ICON_NEURALNETWORK,
	ICON_QLEARNING,
	ICON_NONE
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

	// Victory
	sf::Texture victoryTexture;
	sf::Sprite victorySprite;


	std::map<std::pair<int, int>, Cell*>* currentMap;

	bool setTextureRectFromSemantic(SEMANTIC semantic);

	USER_ACTIONS userAction;

	bool initialized;

	
	bool* lemmingVictory;	// Reference to the lemmingVictory bool in Simulator
	DISPLAY_ICON currentIcon;

public:
	GraphicView(void);

    /**
    *   Initialises the view
    */
	void init(int width, int height, std::map<std::pair<int, int>, Cell*>* map, bool* lemmingVictory);
	void resize(int width, int height);
	void clear();
	sf::RenderWindow* getWindow();

	/**
	*   Draw the world
	*/
	void draw();
	void setIconDisplay(DISPLAY_ICON icon);

	void setWorld(World* world);

	~GraphicView(void);

	// User interface stuff
	void setUserAction(USER_ACTIONS action);
	USER_ACTIONS getUserAction();

	void convertCoordinates_worldToTiles(int px, int py, int& wx, int& wy);
};

#endif
