#include "Simulator.h"

int main(int argc, char* argv[])
{
	Simulator simulatorOfLemming(500);
	simulatorOfLemming.CreateWorld();
	simulatorOfLemming.Run();

	return EXIT_SUCCESS;
}

