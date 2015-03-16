#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <vector>
#include <chrono>
#include <iostream>
#include <tbb/tbb.h>

#include "GraphicView.h"
#include "Agent.h"
#include "World.h"

class Simulator
{
private:
	int numberOfAgents;
	GraphicView SFMLView;
	World world;
	std::vector<Agent> agents;

	Simulator(void) {}

public:
	Simulator(int numberOfAgents);

	void CreateWorld();
	void Run();

	~Simulator(void);
};

#endif