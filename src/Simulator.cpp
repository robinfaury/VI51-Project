#include "Simulator.h"


Simulator::Simulator(int numberOfAgents) : frameFlag(true)
{
	this->numberOfAgents = numberOfAgents;
	this->SFMLView.init(30*27, 30*30, this->world.getMap()->getMap());
}

void Simulator::CreateWorld()
{
	this->world.createMap();

	std::cout << "Map created" << endl;

	// For each body, create an agent
	for (std::vector<Body*>::iterator currentBody = this->world.getBodies()->begin(); currentBody != this->world.getBodies()->end(); ++currentBody)
		this->agents.push_back(new AgentLemmingTest((*currentBody)));

	this->SFMLView.setWorld(&this->world);
}

void Simulator::Run()
{
	std::chrono::system_clock::time_point start_time, end_time;
	int eventID = 0;


	std::cout << "Starting program loop" << endl;
	while(eventID != 1)
	{
		
		if (frameFlag)
		{
			frameFlag = false;
			start_time = std::chrono::high_resolution_clock::now();
			std::cout << endl << "Simulator::Run : STARTING FRAME" << endl;

			// Updating body perceptions

			
			// Running agent
			for (std::vector<Agent*>::iterator currentAgent = this->agents.begin(); currentAgent != this->agents.end(); ++currentAgent)
				(*currentAgent)->live();

			// Updating the world with given influences
			this->world.update();

			// Drawing
			this->SFMLView.draw();


			end_time = std::chrono::high_resolution_clock::now();
			int time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
			std::cout << "frame time : " << time << " ms, waiting " << 1000 - time << " ms" << std::endl;

			std::cout << "Simulator::Run : FRAME ENDED" << endl;
		}
		else
		{
			eventID = this->SFMLView.checkEvent();
			end_time = std::chrono::high_resolution_clock::now();
			int time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
			if (time > 1000)
				frameFlag = true;
		}
	}
}

Simulator::~Simulator(void)
{

}
