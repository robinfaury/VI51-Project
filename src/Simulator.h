#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

#include "GraphicView.h"
#include "World.h"
#include "LearningManager.h"
#include "AgentLemmingDummy.h"
#include "ScriptManager.h"

enum SIMULATION_MODE
{
	SCRIPT,
	LEARNING,
	SIMULATION
};

class Simulator
{
private:
	int numberOfAgents;
	GraphicView SFMLView;
	World world;
	std::vector<Agent*> agents;

	SIMULATION_MODE currentMode;
	LEARNING_TYPE currentIAType;

	LearningManager learningManager;
	ScriptManager scriptManager;

	sf::RenderWindow* window;

	sf::Clock simulationClock;

	// Simulation flag
	bool frameFlag;	// flag used to allow window events while waiting next frame
	bool play;	// flag used to skip simulation at user command
	bool finishSimulation;	// flag used to signal the end of the simulation

	std::string currentLevelPath;
public:
	Simulator(int numberOfAgents);

	World* getWorld();

	void Run();
	void checkEvents();
	void resetSimulation(std::string levelPath = "Islands");
	void recreateAgents();

	void toggleMode(SIMULATION_MODE mode);

	

	// User actions
	void applyUserAction(USER_ACTIONS action, int tileX = -1, int tileY = -1);


};

#endif
