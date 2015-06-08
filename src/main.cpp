#include "Simulator.h"
#include "Problem.h"

int main(int argc, char* argv[]) 
{
	srand (static_cast <unsigned> (time(0)));

	Simulator simulatorOfLemming(1);
	simulatorOfLemming.CreateWorld();

	World* world = simulatorOfLemming.getWorld();
	Problem* problem = new Problem(world->getBodies()->at(0), simulatorOfLemming.getWorld());

	simulatorOfLemming.Run();

	return EXIT_SUCCESS;
}

