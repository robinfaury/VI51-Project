#ifndef GRAPHICVIEW_H_
#define GRAPHICVIEW_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define TILE_SIZE 16
#define LEMMINGTEX_PATH "res/lemming.png"
#define TERRAINTEX_PATH "res/terrain.png"

#include "World.h"

class GraphicView
{
private:
	sf::RenderWindow* window;
	World* world;

	sf::Texture terrainTexture;
	sf::Texture lemmingTexture;

	sf::Sprite terrainSprite;
	sf::Sprite lemmingSprite;

	std::map<std::pair<int, int>, Cell*>* currentMap;

	bool setTextureRectFromSemantic(Semantic* semantic);

public:
	GraphicView(void);

	void init(int height, int width);
	int checkEvent();
	void checkWorldEvents();
	void draw();

	void setWorld(World* world);

	~GraphicView(void);
};

#endif
