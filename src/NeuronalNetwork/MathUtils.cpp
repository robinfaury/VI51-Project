#include "MathUtils.h"

float sigmoide(float x)
{
	return 1.0f/(1.0f+exp(-4*x));
}
