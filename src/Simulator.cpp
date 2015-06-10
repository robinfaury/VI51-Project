#include "Simulator.h"


Simulator::Simulator(int numberOfAgents) : currentLevelPath("Islands"), world(&this->currentLevelPath), learningManager(&this->world), frameFlag(true), play(false), finishSimulation(false), currentMode(SIMULATION_MODE::LEARNING), currentIAType(LEARNING_TYPE::NEURALNETWORK)

{
	this->numberOfAgents = numberOfAgents;

	this->SFMLView.setWorld(&this->world);
	this->toggleMode(SIMULATION_MODE::SIMULATION);
}

World* Simulator::getWorld()
{
	return &this->world;
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
			//std::cout << endl << "Simulator::Run : STARTING FRAME" << endl;

			if (play)
			{
				// Updating body perceptions
				this->world.setPerceptions();

				// Running agent
				int i = 0;
				for (std::vector<Agent*>::iterator currentAgent = this->agents.begin(); currentAgent != this->agents.end(); ++currentAgent)
				{
					(*currentAgent)->live();
					std::cout << "Agent " << i << " living" << std::endl;
					++i;
				}
					

				// Updating the world with given influences
				this->world.update();
			}

			// Drawing
			this->SFMLView.draw();

			endTime = this->simulationClock.getElapsedTime();
			frameTime = endTime - startTime;

			if (frameTime.asMilliseconds() > 100)
				std::cout << "PERFORMANCE ISSUE : frame time : " << frameTime.asMilliseconds() << std::endl;
			//std::cout << "Simulator::Run : FRAME ENDED" << endl;
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
				{
					this->play = true;
					std::cout << "Simulation : play" << std::endl;
				}
				else
				{
					this->play = false;
					std::cout << "Simulation : pause" << std::endl;
				}
					
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				this->window->close();
				finishSimulation = true;
			}
			else if (event.key.code == sf::Keyboard::S)
			{
				//Save level
				std::cout << "Saving : Please input level name : ";
				std::string path;
				std::cin >> path;
				this->world.saveLevel(path);
			}
			else if (event.key.code == sf::Keyboard::L)
			{
				//Load level
				std::cout << "Loading : Please input level name : ";
				std::string path;
				std::cin >> path;
				
				this->resetSimulation(path);
			}
			else if (event.key.code == sf::Keyboard::R)
			{
				// Forcing pause
				this->play = false;

				//Load level
				std::cout << "Resetting level";

				this->resetSimulation(this->currentLevelPath);
			}
			else if (event.key.code == sf::Keyboard::F2)
			{
				this->toggleMode(SIMULATION_MODE::SIMULATION);
			}
			else if (event.key.code == sf::Keyboard::F1)
			{
				this->toggleMode(SIMULATION_MODE::LEARNING);
			}
			else if (event.key.code == sf::Keyboard::F5)
			{
				this->currentIAType = LEARNING_TYPE::QLEARNING;
				recreateAgents();
				std::cout << "Currently selected agents : QLearning" << std::endl;
			}
			else if (event.key.code == sf::Keyboard::F6)
			{
				this->currentIAType = LEARNING_TYPE::NEURALNETWORK;
				recreateAgents();
				std::cout << "Currently selected agents : NeuralNetwork" << std::endl;
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

void Simulator::resetSimulation(std::string levelPath)
{
	//TODO: handle error in world loading
	

	if (!this->world.loadLevel(levelPath))
	{
		std::cout << "ERROR : Simulator::resetSimulation : World could not load map. Switching to default map..." << std::endl;
		this->currentLevelPath = "Islands";
		this->world.loadLevel(this->currentLevelPath);
	}
	else
	{
		this->currentLevelPath = levelPath;
	}
	recreateAgents();
	
}

void Simulator::recreateAgents()
{
	// Clearing agents, and reaffecting them
	this->agents.clear();

	// For each body, give it an agent
	std::vector<Body*>* bodies = this->world.getBodies();
	Agent* tempAgent = NULL;
	for (std::vector<Body*>::iterator it = this->world.getBodies()->begin(); it != this->world.getBodies()->end(); ++it)
	{
		tempAgent = this->learningManager.getAgent(this->currentIAType);
		std::cout << "Trying to get agent for method " << this->currentIAType << std::endl;
		if (tempAgent == NULL)
			std::cout << "ERROR : Simulator::CheckEvents : couldn't reassign new agents after loading level" << std::endl;
		else
		{
			tempAgent->linkBody(*it);
			this->agents.push_back(tempAgent);
		}
	}
}

void Simulator::toggleMode(SIMULATION_MODE mode)
{
    if (mode == SIMULATION_MODE::SIMULATION && this->currentMode != SIMULATION_MODE::SIMULATION)
    {
		this->resetSimulation(this->currentLevelPath);

		int height = TILE_SIZE*HEIGHT;
		int width = TILE_SIZE*WIDTH;
        this->SFMLView.init(500, 500, this->world.getMap()->getMap());
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
