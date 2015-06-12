#include "Simulator.h"


Simulator::Simulator(int numberOfAgents) : currentLevelPath("Islands"), world(&this->currentLevelPath), learningManager(&this->world), frameFlag(true), play(false), finishSimulation(false), 
currentMode(SIMULATION_MODE::LEARNING), currentIAType(LEARNING_TYPE::NEURALNETWORK), scriptManager(&this->learningManager, &this->world, currentLevelPath)
{
	this->numberOfAgents = numberOfAgents;

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
			std::string path;	// Forward declaration for the std::cin
			int intChoice;

			switch (event.key.code)
			{
			case sf::Keyboard::P :
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
				break;
			case sf::Keyboard::C:
				std::cout << "Current lemming state : " << Problem::convertPerceptionToStateId(this->world.getBodies()->at(0)->getPerception(), this->world.getSize()) << std::endl;
				break;
			case sf::Keyboard::Escape:
				this->window->close();
				finishSimulation = true;
				break;

				//TODO: remove that
			case sf::Keyboard::V:
				if (this->world.lemmingSuccess())
					std::cout << "LEMMING SUCCESS" << std::endl;
				else
					std::cout << "LEMMING fail" << std::endl;
				break;
				
			case sf::Keyboard::S:
				//Save level
				path.clear();
				do
				{
                    HelperFunctions::safeChoice("Saving : Please input level name : ", "Please enter a valid name", path);
				} while (path.empty());

				this->world.saveLevel(path);
				break;
			case sf::Keyboard::L:
				//Load level
				path.clear();
				HelperFunctions::safeChoice("Loading : Please input level name : ", "Please enter a valid name", path);

				this->resetSimulation(path);
				this->SFMLView.resize(800, 800);
				break;
			case sf::Keyboard::R:
				// Forcing pause
				this->play = false;

				//Load level
				std::cout << "Resetting level";

				this->resetSimulation(this->currentLevelPath);
				break;
			
			case sf::Keyboard::F1:
				this->toggleMode(SIMULATION_MODE::LEARNING);
				break;
			case sf::Keyboard::F2:
				this->toggleMode(SIMULATION_MODE::SIMULATION);
				break;
			case sf::Keyboard::F3:
				this->toggleMode(SIMULATION_MODE::SCRIPT);
				this->toggleMode(SIMULATION_MODE::SIMULATION);

				break;
			case  sf::Keyboard::F5:
				this->currentIAType = LEARNING_TYPE::QLEARNING;
				recreateAgents();
				std::cout << "Currently selected agents : QLearning" << std::endl;
				break;
			case sf::Keyboard::F6:
				this->currentIAType = LEARNING_TYPE::NEURALNETWORK;
				std::cout << "Currently selected agents : NeuralNetwork" << std::endl;
				recreateAgents();
				break;
			case sf::Keyboard::A:
				this->SFMLView.setUserAction(USER_ACTIONS::U_CLEAR);
				std::cout << "Selected CLEAR tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::Z:
				this->SFMLView.setUserAction(USER_ACTIONS::U_DIRT);
				std::cout << "Selected DIRT tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::E:
				this->SFMLView.setUserAction(USER_ACTIONS::U_ROCK);
				std::cout << "Selected ROCK tile : click to apply" << std::endl;
				break;
			case sf::Keyboard::M :
                HelperFunctions::safeChoice("Input new map size : ", "Please enter a valid int", intChoice);

				this->world.setSize(intChoice);
				resetSimulation("Generate");
				this->SFMLView.resize(800, 800);
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
		if (tempAgent == NULL)
			std::cout << "ERROR : Simulator::CheckEvents : couldn't reassign new agents after loading level" << std::endl;
		else
		{

			tempAgent->linkBody(*it);
			this->agents.push_back(tempAgent);
			std::cout << "Created another agent" << std::endl;
		}
	}
	std::cout << "Agents recreated" << std::endl;
}

void Simulator::toggleMode(SIMULATION_MODE mode)
{
    if (mode == SIMULATION_MODE::SIMULATION && this->currentMode != SIMULATION_MODE::SIMULATION)
    {
		this->resetSimulation(this->currentLevelPath);

		int height = TILE_SIZE * this->world.getSize();
		int width = TILE_SIZE * this->world.getSize();
        this->SFMLView.init(600, 600, this->world.getMap()->getMap());
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
		this->scriptManager.setAlpha(0.2, 0.4, 0.1);
		this->scriptManager.setGamma(0.7, 0.8, 0.05);
		this->scriptManager.setRho(0.1, 0.3, 0.1);
		this->scriptManager.setNu(0.05, 0.15, 0.05);
		this->scriptManager.setTriesPerLearning(10);
		this->scriptManager.setIterations(100, 300, 100);

		this->scriptManager.addMapToPool("10_Easy1");
		this->scriptManager.addMapToPool("10_Medium1");
		this->scriptManager.addMapToPool("10_Hard1");

		// Common map pool
		//10*10
		/*this->scriptManager.addMapToPool("10_Easy1");
		this->scriptManager.addMapToPool("10_Easy2");
		this->scriptManager.addMapToPool("10_Easy3");
		this->scriptManager.addMapToPool("10_Medium1");
		this->scriptManager.addMapToPool("10_Medium2");
		this->scriptManager.addMapToPool("10_Medium3");
		this->scriptManager.addMapToPool("10_Hard1");
		this->scriptManager.addMapToPool("10_Hard2");
		this->scriptManager.addMapToPool("10_Hard3");

		//15*15
		this->scriptManager.addMapToPool("15_Easy1");
		this->scriptManager.addMapToPool("15_Easy2");
		this->scriptManager.addMapToPool("15_Medium1");
		this->scriptManager.addMapToPool("15_Medium2");
		this->scriptManager.addMapToPool("15_Medium3");
		this->scriptManager.addMapToPool("15_Medium4");
		this->scriptManager.addMapToPool("15_Hard1");
		this->scriptManager.addMapToPool("15_Hard2");
		this->scriptManager.addMapToPool("15_Hard3");

		//20*20
		this->scriptManager.addMapToPool("20_Easy1");
		this->scriptManager.addMapToPool("20_Easy2");
		this->scriptManager.addMapToPool("20_Easy3");
		this->scriptManager.addMapToPool("20_Medium1");
		this->scriptManager.addMapToPool("20_Medium2");
		this->scriptManager.addMapToPool("20_Medium3");
		this->scriptManager.addMapToPool("20_Hard1");
		this->scriptManager.addMapToPool("20_Hard2");
		this->scriptManager.addMapToPool("20_Hard3");

		// parameters
		this->scriptManager.setAlpha(0.1, 0.5, 0.05);
		this->scriptManager.setGamma(0.5, 0.9, 0.05);
		this->scriptManager.setRho(0.1, 0.5, 0.05);
		this->scriptManager.setNu(0.01, 0.2, 0.01);
		this->scriptManager.setTriesPerLearning(1);*/


						// TODO : decommentez la partie correspondante, puis lancez l'exécution, puis faites F3. Et après, ne touchez à rien! Vous pouvez vérifier sur la console que c'est bien lancé.
			// Aiguille
		/*this->scriptManager.setIterations(10000, 90000, 100000);
		this->scriptManager.launchScript("MegaTest_Aiguille");*/

			// Amarre notebook
		/*this->scriptManager.setIterations(20000, 80000, 100000);
		this->scriptManager.launchScript("MegaTest_AmarreNotebook");*/

			// Golé portable
		/*this->scriptManager.setIterations(30000, 70000, 100000);
		this->scriptManager.launchScript("MegaTest_GolePortable");*/

			// Amarre portable
		/*this->scriptManager.setIterations(50000, 60000, 10000);
		this->scriptManager.launchScript("MegaTest_AmarrePortable");*/

			// Golé fixe
		/*this->scriptManager.setIterations(40000, 100000, 60000);
		this->scriptManager.launchScript("MegaTest_GoleFixe");*/

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
	}
}
