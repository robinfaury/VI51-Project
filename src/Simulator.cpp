#include "Simulator.h"


Simulator::Simulator(int numberOfAgents) : world(), learningManager(&this->world), frameFlag(true), play(true), finishSimulation(false), currentMode(SIMULATION_MODE::LEARNING)
{
	this->numberOfAgents = numberOfAgents;

	this->toggleMode(SIMULATION_MODE::SIMULATION);
}

World* Simulator::getWorld()
{
	return &this->world;
}

void Simulator::CreateWorld()
{
	this->world.createMap();

	std::cout << "Map created" << endl;



	// For each body, create an agent
	for (std::vector<Body*>::iterator currentBody = this->world.getBodies()->begin(); currentBody != this->world.getBodies()->end(); ++currentBody)
		this->agents.push_back(new AgentLemmingDummy(*currentBody));

	this->SFMLView.setWorld(&this->world);
}

void Simulator::Run()
{
	this->simulationClock.restart();

	sf::Time startTime, endTime, frameTime;

	std::cout << "Starting program loop" << endl;
	while(!finishSimulation)
	{
		if (this->currentMode == SIMULATION_MODE::SIMULATION && this->frameFlag)
		{
			this->frameFlag = false;

			startTime = this->simulationClock.getElapsedTime();
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

			endTime = this->simulationClock.getElapsedTime();
			frameTime = endTime - startTime;

			std::cout << "frame time : " << frameTime.asMilliseconds() << std::endl;
			std::cout << "Simulator::Run : FRAME ENDED" << endl;
        }
		else if (currentMode == SIMULATION_MODE::LEARNING)
		{
            std::cout << std::endl << "Starting learning mode" << std::endl;
            // Force pause
            this->play = false;
            //Learn with all methods
            std::cout << "Simulator : Performing learning" << std::endl;
            this->learningManager.launchLearning();
            endTime = this->simulationClock.getElapsedTime();
			frameTime = endTime - startTime;

			std::cout << "Learning complete! Total learning time : " << frameTime.asMilliseconds() << std::endl;

            this->learningManager.displayReports();

            std::cout << std::endl << "Resuming to simulation" << std::endl;
            this->toggleMode(SIMULATION_MODE::SIMULATION);
		}
		else
		{
			checkEvents();	// Checking for user input

			endTime = this->simulationClock.getElapsedTime();
			frameTime = endTime - startTime;

			if (play)
			{
				if (frameTime.asMilliseconds() > 200)
					frameFlag = true;
			}
			else
			{
				if (frameTime.asMilliseconds() > 100)
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
			else if (event.key.code == sf::Keyboard::F2)
			{
				//TODO: launch simulation
				this->toggleMode(SIMULATION_MODE::SIMULATION);
			}
			else if (event.key.code == sf::Keyboard::F1)
			{
				//TODO: launch learning
				this->toggleMode(SIMULATION_MODE::LEARNING);
			}
			else if (event.key.code == sf::Keyboard::A)
				this->SFMLView.setUserAction(USER_ACTIONS::U_CLEAR);
			else if (event.key.code == sf::Keyboard::Z)
				this->SFMLView.setUserAction(USER_ACTIONS::U_DIRT);
			else if (event.key.code == sf::Keyboard::E)
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

void Simulator::toggleMode(SIMULATION_MODE mode)
{
    if (mode == SIMULATION_MODE::SIMULATION && this->currentMode != SIMULATION_MODE::SIMULATION)
    {
		int height = TILE_SIZE*HEIGHT;
		int width = TILE_SIZE*WIDTH;
        this->SFMLView.init(height, width, this->world.getMap()->getMap());
        this->window = this->SFMLView.getWindow();
    }

    else if (mode != SIMULATION_MODE::SIMULATION && this->currentMode == SIMULATION_MODE::SIMULATION)
        this->SFMLView.clear();

    this->currentMode = mode;

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
