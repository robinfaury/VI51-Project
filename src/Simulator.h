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

enum SIMULATION_MODE
{
	MENU = 0,
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

	LearningManager learningManager;

	sf::RenderWindow* window;

	sf::Clock simulationClock;

	// Simulation flag
	bool frameFlag;	// flag used to allow window events while waiting next frame
	bool play;	// flag used to skip simulation at user command
	bool finishSimulation;	// flag used to signal the end of the simulation
public:
	Simulator(int numberOfAgents);

	World* getWorld();

	void CreateWorld();
	void Run();
	void checkEvents();

	void toggleMode(SIMULATION_MODE mode);

	// User actions
	void applyUserAction(USER_ACTIONS action, int tileX = -1, int tileY = -1);
};

#endif
