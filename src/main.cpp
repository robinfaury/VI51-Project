#include "Simulator.h"
#include "Problem.h"

int main(int argc, char* argv[]) 
{
	Simulator simulatorOfLemming(1);
	simulatorOfLemming.CreateWorld();
	simulatorOfLemming.Run();

	//World* world = simulatorOfLemming.getWorld();
	//Problem* problem = new Problem(world->getBodies()->at(0), simulatorOfLemming.getWorld());

	return EXIT_SUCCESS;
}

