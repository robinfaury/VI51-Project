#include "Simulator.h"
#include "Problem.h"

int main(int argc, char* argv[]) 
{
	srand (static_cast <unsigned> (time(0)));

	Simulator simulatorOfLemming(1);
	simulatorOfLemming.Run();

	return EXIT_SUCCESS;
}

