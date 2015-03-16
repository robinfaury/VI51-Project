#include "Simulator.h"


Simulator::Simulator(int numberOfAgents)
{
	this->numberOfAgents = numberOfAgents;
	this->SFMLView.Init(800, 800);
}

void Simulator::CreateWorld()
{
	for (int i=0; i<this->numberOfAgents; ++i)
	{
		Body* body = this->world.CreateBody();
		this->agents.push_back(Agent(body));
	}

	this->SFMLView.SetWorld(&this->world);
}

void Simulator::Run()
{
	std::chrono::system_clock::time_point start_time, end_time;
	int eventID = 0;

	while(eventID != 1)
	{
		start_time = std::chrono::high_resolution_clock::now();

		eventID = this->SFMLView.CheckEvent();

		int nbAgents = static_cast<int>(agents.size());
		tbb::parallel_for(0, nbAgents, 
			[&](int i) 
			{
				this->agents[i].Life();
			}
		);

		this->SFMLView.Draw();

		end_time = std::chrono::high_resolution_clock::now();
		std::cout <<"frame time : "<< std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << std::endl;
	}
}

Simulator::~Simulator(void)
{
}
