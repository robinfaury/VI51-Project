#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

#include "GraphicView.h"
#include "AgentLemmingTest.h"
#include "World.h"

class Simulator
{
private:
	int numberOfAgents;
	GraphicView SFMLView;
	World world;
	std::vector<Agent*> agents;


	bool frameFlag;	// flag used to allow window events while waiting next frame
public:
	Simulator(int numberOfAgents);

	void CreateWorld();
	void Run();

	~Simulator(void);
};

#endif