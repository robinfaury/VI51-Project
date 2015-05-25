#include "Simulator.h"

int main(int argc, char* argv[]) 
{
	Simulator simulatorOfLemming(1);
	simulatorOfLemming.CreateWorld();
	simulatorOfLemming.Run();

	return EXIT_SUCCESS;
}

