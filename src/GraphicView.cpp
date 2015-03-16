#include "GraphicView.h"


GraphicView::GraphicView(void)
{
}

void GraphicView::Init(int height, int width)
{
	this->window = new sf::RenderWindow(sf::VideoMode(height, width), "VI51 - Simulator of Lemming");
	this->window->setVerticalSyncEnabled(true);
}

int GraphicView::CheckEvent()
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

void GraphicView::Draw()
{
	window->clear(sf::Color::Black);

	std::vector<Body*>* listOfBodys = this->world->GetListOfBodys();
	if (this->bodyShape.size() != listOfBodys->size())
	{
		this->bodyShape.clear();
		for (int i=0; i<listOfBodys->size(); ++i)
		{
			this->bodyShape.push_back(sf::CircleShape(2));
			this->bodyShape.at(this->bodyShape.size()-1).setFillColor(sf::Color(255, 255, 255));
		}
	}

	for (int idCurrentBody=0; idCurrentBody<listOfBodys->size(); ++idCurrentBody)
	{
		std::vector<float> pos = listOfBodys->at(idCurrentBody)->GetPosition();
		this->bodyShape[idCurrentBody].setPosition(pos[0], pos[1]);
		window->draw(this->bodyShape[idCurrentBody]);
	}

	window->display();
}

void GraphicView::SetWorld(World* world)
{
	this->world = world;
}

GraphicView::~GraphicView(void)
{
	this->window->clear();
	delete this->window;
}
