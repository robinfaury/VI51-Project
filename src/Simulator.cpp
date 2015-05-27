#include "Simulator.h"


Simulator::Simulator(int numberOfAgents) : frameFlag(true), play(true), finishSimulation(false)
{
	this->numberOfAgents = numberOfAgents;
	this->SFMLView.init(30*27, 30*30, this->world.getMap()->getMap());
	this->window = this->SFMLView.getWindow();
}

void Simulator::CreateWorld()
{
	this->world.createMap();

	std::cout << "Map created" << endl;

	// For each body, create an agent
	for (std::vector<Body*>::iterator currentBody = this->world.getBodies()->begin(); currentBody != this->world.getBodies()->end(); ++currentBody)
		this->agents.push_back(new AgentLemmingTest((*currentBody)));

	this->SFMLView.setWorld(&this->world);
}

void Simulator::Run()
{
	std::chrono::system_clock::time_point start_time, end_time;


	std::cout << "Starting program loop" << endl;
	while(!finishSimulation)
	{
		
		if (this->frameFlag)
		{
			this->frameFlag = false;
			start_time = std::chrono::high_resolution_clock::now();
			std::cout << endl << "Simulator::Run : STARTING FRAME" << endl;

			if (play)
			{
				// Updating body perceptions
				this->world.setPerceptions();

				// Running agent
				for (std::vector<Agent*>::iterator currentAgent = this->agents.begin(); currentAgent != this->agents.end(); ++currentAgent)
					(*currentAgent)->live();

				// Updating the world with given influences
				this->world.update();
			}

			// Drawing
			this->SFMLView.draw();

			end_time = std::chrono::high_resolution_clock::now();
			__int64 time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
			std::cout << "frame time : " << time << " ms, waiting " << 1000 - time << " ms" << std::endl;

			std::cout << "Simulator::Run : FRAME ENDED" << endl;
		}
		else
		{
			checkEvents();	// Checking for user input

			end_time = std::chrono::high_resolution_clock::now();
			__int64 time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

			if (play)
			{
				if (time > 1000)
					frameFlag = true;
			}
			else
			{
				if (time > 100)
					frameFlag = true;
			}
		}
	}
}

void Simulator::checkEvents()
{
	// Checking for window events
	sf::Event event;
	while (this->window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->window->close();
			finishSimulation = true;
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::P)
			{
				if (!this->play)
					this->play = true;
				else
					this->play = false;
			}
			else if (event.key.code == sf::Keyboard::F1)
				this->SFMLView.setUserAction(USER_ACTIONS::U_CLEAR);
			else if (event.key.code == sf::Keyboard::F2)
				this->SFMLView.setUserAction(USER_ACTIONS::U_DIRT);
			else if (event.key.code == sf::Keyboard::F3)
				this->SFMLView.setUserAction(USER_ACTIONS::U_ROCK);
		}
	}

	// Checking user input
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// Left click : painting the map
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*this->window);
		sf::Vector2f worldPos = this->window->mapPixelToCoords(pixelPos);
		int x, y;
		this->SFMLView.convertCoordinates_worldToTiles(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y), x, y);

		applyUserAction(this->SFMLView.getUserAction(), x, y);
	}
}

void Simulator::applyUserAction(USER_ACTIONS action, int tileX, int tileY)
{
	switch (action)
	{
	case USER_ACTIONS::U_CLEAR :
		this->world.removeObject(tileX, tileY);
		break;
	case USER_ACTIONS::U_DIRT :
		this->world.removeObject(tileX, tileY);
		this->world.createObject(tileX, tileY, SEMANTIC::T_DIRT);
		break;
	case USER_ACTIONS::U_ROCK:
		this->world.removeObject(tileX, tileY);
		this->world.createObject(tileX, tileY, SEMANTIC::T_ROCK);
		break;
	}
}