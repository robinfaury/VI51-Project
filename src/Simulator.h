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
	GraphicView SFMLView;	// Graphics interface
	World world;	// World
	std::vector<Agent*> agents;	// Container of agents

	SIMULATION_MODE currentMode;	// Current simulation mode
	LEARNING_TYPE currentIAType;	// Currently selected AI

	LearningManager learningManager;	// Learning Manager
	ScriptManager scriptManager;		// Script manager

	sf::RenderWindow* window;	// Pointer to the graphics interface's window

	sf::Clock simulationClock;	// Simulation clock

	// Simulation flag
	bool frameFlag;	// flag used to allow window events while waiting next frame
	bool play;	// flag used to skip simulation at user command
	bool finishSimulation;	// flag used to signal the end of the simulation

	std::string currentLevelPath;	// Path of the current level
	bool lemmingVictory;	// is set to true if lemming completed the map
public:
	Simulator();
	~Simulator();
	void clearSimulator();

	// World getter function
	World* getWorld();

	// main application loop
	void Run();

	// Checks for user input, and applies it
	void checkEvents();

	// Resets simulation to given level path. If no path is provided, load default map.
	void resetSimulation(std::string levelPath = "Islands");
	// Reset agents with current simulation mode
	void recreateAgents();

	// Toggle simulation mode
	void toggleMode(SIMULATION_MODE mode);

	// Apply user action on the world
	void applyUserAction(USER_ACTIONS action, int tileX = -1, int tileY = -1);
};

#endif
