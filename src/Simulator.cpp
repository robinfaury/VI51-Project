#include "Simulator.h"


Simulator::Simulator(int numberOfAgents)
{
	this->numberOfAgents = numberOfAgents;
	this->SFMLView.init(30*27, 30*30);
}

void Simulator::CreateWorld()
{
	this->world.createMap();

	for (std::vector<Body*>::iterator currentBody = this->world.getBodies()->begin(); currentBody != this->world.getBodies()->end(); ++currentBody)
		this->agents.push_back(Agent((*currentBody)));

	this->SFMLView.setWorld(&this->world);
}

void Simulator::Run()
{
	std::chrono::system_clock::time_point start_time, end_time;
	int eventID = 0;

	while(eventID != 1)
	{
		start_time = std::chrono::high_resolution_clock::now();

		eventID = this->SFMLView.checkEvent();

		for (std::vector<Agent>::iterator currentAgent = this->agents.begin(); currentAgent != this->agents.end(); ++currentAgent)
			(*currentAgent).live();

    //TODO: fix that
		//this->world.collectInfluence();
		this->world.resolveInfluences();

		this->SFMLView.draw();

		end_time = std::chrono::high_resolution_clock::now();
		std::cout <<"frame time : "<< std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << std::endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

Simulator::~Simulator(void)
{
}
