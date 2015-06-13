#include "Simulator.h"


Simulator::Simulator() : currentLevelPath("Islands"), world(&this->currentLevelPath), learningManager(&this->world), frameFlag(true), play(false), finishSimulation(false), 
currentMode(SIMULATION_MODE::LEARNING), currentIAType(LEARNING_TYPE::NEURALNETWORK), scriptManager(&this->learningManager, &this->world, currentLevelPath), lemmingVictory(false)
{

	this->SFMLView.setWorld(&this->world);
	this->toggleMode(SIMULATION_MODE::SIMULATION);
}

Simulator::~Simulator()
{
	clearSimulator();
}

void Simulator::clearSimulator()
{
	for (std::vector<Agent*>::iterator it = this->agents.begin(); it != this->agents.end(); ++it)
	{
		delete(*it);
		*it = NULL;
	}
	this->agents.clear();

	this->learningManager.clearLearningManager();
	this->scriptManager.clearScriptManager();
	this->world.clearWorld();
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


				// Updating the world (collecting and resolving influences)
				this->world.update();
				if (this->world.lemmingSuccess())
				{
					this->lemmingVictory = true;
					play = false;
				}
					
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

			// Save the map, to allow Learning methods to reload it
			this->currentLevelPath = "LearningPath";
			this->world.saveLevel(currentLevelPath);


            //Learn with all methods
            std::cout << "Simulator : Performing learning" << std::endl;
            this->learningManager.launchLearning();
            endTime = this->simulationClock.getElapsedTime();
			frameTime = endTime - startTime;

			std::cout << "Learning complete! Total learning time : " << frameTime.asSeconds() << std::endl;

            this->learningManager.displayReports();

            std::cout << std::endl << "Resuming to simulation" << std::endl;
            this->toggleMode(SIMULATION_MODE::SIMULATION);
		}
		else
		{
			checkEvents();	// Checking for user input

			// Calculating elapsed time
			endTime = this->simulationClock.getElapsedTime();
			frameTime = endTime - startTime;

			// If simulation is playing, update every 1/5th of a second
			if (play)
			{
				if (frameTime.asMilliseconds() > 200)
					frameFlag = true;
			}
			else // update every 1/10th of a second
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
		// Window closed
		if (event.type == sf::Event::Closed)
		{
			this->window->close();
			finishSimulation = true;
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			std::string path;	// Forward declaration for the std::cin
			int intChoice;

			// Keyboard events
			switch (event.key.code)
			{
						// SIMULATION MANAGEMENT
				// Select QLearning AI type
			case  sf::Keyboard::F5:
				this->currentIAType = LEARNING_TYPE::QLEARNING;
				recreateAgents();
				this->SFMLView.setIconDisplay(DISPLAY_ICON::ICON_QLEARNING);
				std::cout << "Currently selected agents : QLearning" << std::endl;
				break;
				// Select neural network AI type
			case sf::Keyboard::F6:
				this->currentIAType = LEARNING_TYPE::NEURALNETWORK;
				this->SFMLView.setIconDisplay(DISPLAY_ICON::ICON_NEURALNETWORK);
				recreateAgents();
				std::cout << "Currently selected agents : NeuralNetwork" << std::endl;
				break;

				// Play / pause simulation
			case sf::Keyboard::P :
				if (!this->play)
				{
					if ((this->lemmingVictory))	// If simulation is paused due to victory, reset it
					{
						this->resetSimulation(this->currentLevelPath);
					}
					else
						std::cout << "Simulation : play" << std::endl;
					this->play = true;
				}
				else
				{
					this->play = false;
					std::cout << "Simulation : pause" << std::endl;
				}
				break;

				// reset the simulation
			case sf::Keyboard::R:
				// Forcing pause
				this->play = false;

				//Load level
				std::cout << "Resetting level";

				this->resetSimulation(this->currentLevelPath);
				break;

				// Close the window
			case sf::Keyboard::Escape:
				this->window->close();
				finishSimulation = true;
				break;

					// MAP MANAGEMENT
				// Save the current level
			case sf::Keyboard::S:
				//Save level
				path.clear();
				do
				{
                    HelperFunctions::safeChoice("Saving : Please input level name : ", "Please enter a valid name", path);
				} while (path.empty());

				this->world.saveLevel(path);
				break;
				// Load a level
			case sf::Keyboard::L:
				//Load level
				path.clear();
				HelperFunctions::safeChoice("Loading : Please input level name : ", "Please enter a valid name", path);

				this->resetSimulation(path);
				this->SFMLView.resize(800, 800);
				break;
				// Choose map size
			case sf::Keyboard::M:
				HelperFunctions::safeChoice("Input new map size : ", "Please enter a valid int", intChoice);

				this->world.setSize(intChoice);
				resetSimulation("Generate");
				this->SFMLView.resize(800, 800);
				break;

				// Terrain types selection
			case sf::Keyboard::W:
				this->SFMLView.setUserAction(USER_ACTIONS::U_CLEAR);
				std::cout << "Selected EMPTY tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::X:
				this->SFMLView.setUserAction(USER_ACTIONS::U_DIRT);
				std::cout << "Selected DIRT tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::C:
				this->SFMLView.setUserAction(USER_ACTIONS::U_ROCK);
				std::cout << "Selected ROCK tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::V:
				this->SFMLView.setUserAction(USER_ACTIONS::U_EXIT);
				std::cout << "Selected EXIT tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::B:
				this->SFMLView.setUserAction(USER_ACTIONS::U_BOUND);
				std::cout << "Selected BOUND tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::N:
				this->SFMLView.setUserAction(USER_ACTIONS::U_NONE);
				std::cout << "Selection deactivated" << std::endl;
				break;


					// SIMULATION MODES
				// Launch learning mode
			case sf::Keyboard::F1:
				this->toggleMode(SIMULATION_MODE::LEARNING);
				break;
				// Launch simulation mode
			case sf::Keyboard::F2:
				this->toggleMode(SIMULATION_MODE::SIMULATION);
				break;
				// Launch script mode
			case sf::Keyboard::F3:
				this->toggleMode(SIMULATION_MODE::SCRIPT);
				this->toggleMode(SIMULATION_MODE::SIMULATION);
				break;


					// QLEARNING MANAGEMENT
				// Print the corresponding state (QLearning state) the lemming is currently in.
			case sf::Keyboard::Q:
				std::cout << "Current lemming state : " << Problem::convertPerceptionToStateId(this->world.getBodies()->at(0)->getPerception(), this->world.getSize()) << std::endl;
				break;
			}
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

	this->play = false;
	this->lemmingVictory = false;
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
		if (tempAgent == NULL)
		{

		}
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

		int height = TILE_SIZE * this->world.getSize();
		int width = TILE_SIZE * this->world.getSize();
        this->SFMLView.init(600, 600, this->world.getMap()->getMap(), &lemmingVictory);
        this->window = this->SFMLView.getWindow();

		recreateAgents();
    }

    else if (mode != SIMULATION_MODE::SIMULATION && this->currentMode == SIMULATION_MODE::SIMULATION)
        this->SFMLView.clear();


	switch (mode)
	{
	case SCRIPT:
		// Clearing scriptManager
		this->scriptManager.clearMapPool();
		this->scriptManager.clearSpecialIterationNumbers();

		// setting parameters
		// Testing the script
		this->scriptManager.setAlpha(0.3f, 0.3f, 0.1f);
		this->scriptManager.setGamma(0.75f, 0.75f, 0.75f);
		this->scriptManager.setRho(0.2f, 0.2f, 0.1f);
		this->scriptManager.setNu(0.05f, 0.15f, 0.01f);
		this->scriptManager.setTriesPerLearning(100);
		this->scriptManager.setIterations(5000, 50000, 5000);

		this->scriptManager.addMapToPool("15_Hard5");
		this->scriptManager.launchScript("Test_nu_15Hard4");

		break;
	default:
		break;
	}
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
	case USER_ACTIONS::U_EXIT:
		this->world.removeObject(tileX, tileY);
		this->world.createObject(tileX, tileY, SEMANTIC::T_EXIT);
		break;
	case USER_ACTIONS::U_BOUND:
		this->world.removeObject(tileX, tileY);
		this->world.createObject(tileX, tileY, SEMANTIC::T_BOUND);
		break;
	}
}
