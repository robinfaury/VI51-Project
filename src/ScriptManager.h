#ifndef SCRIPTMANAGER_H_
#define SCRIPTMANAGER_H_

/*
The script manager is aimed at the QLearning algorithm.
Its goal is to execute the learning algorithm with a given set of parameters, then test the results on a map.
Map and parameters can be set to vary by a given variation value between a min and max value.
*/

#define scriptResultsPath "scriptResults/"
#define SCRIPTMANAGER_DEBUG 0

#include "LearningManager.h"
#include "World.h"

class ScriptManager
{
public:
	ScriptManager(LearningManager* learningManager, World* world, std::string& currentLevelPath);
	~ScriptManager();
	void clearScriptManager();

	void launchScript(std::string name);	// the main script. Parameters need to be set first. (And be prepared for A LONG RUNTIME)
	void testingCurrentScriptingState(int triesPerScriptingState, int maxLength, float& percentageOfSuccess, float& averageSuccessLength, float& numberOfSuccesses);	// For the current map, tests the lemming's ability to complete the map
	void resetAgent();	// Resets the agent attribute

	// Parameter configuration
	void addMapToPool(std::string mapName);
	void clearMapPool();

	void addSpecialIterationNumber(int specialIterationNumber);
	void clearSpecialIterationNumbers();

	void setAlpha(float minAlpha, float maxAlpha, float alphaVariation);
	void setGamma(float minGamma, float maxGamma, float gammaVariation);
	void setRho(float minRho, float maxRho, float rhoVariation);
	void setNu(float minNu, float maxNu, float nuVariation);
	void setIterations(int minIterations, int maxIterations, int iterationsVariation);

	void setTriesPerLearning(int triesPerLearning);

	// Resets the map (for a new test)
	void resetMap(std::string levelPath);
	
	// Serialize the results of the current test
	void serializeCurrentResult(pugi::xml_node* node, float alpha, float gamma, float rho, float nu, int iterations, float percentageOfSuccess, float averageSuccessLength, float numberOfSuccesses);

protected:
	LearningManager* learningManager;
	QLearning* qLearning;
	World* world;
	Agent* agent;
	std::string& currentLevelPath;	// The world uses the simulation currentLevelPath, so the scriptManager needs to modify it

	// Parameter values and their variations
	float minAlpha, maxAlpha, alphaVariation;
	float minGamma, maxGamma, gammaVariation;
	float minRho, maxRho, rhoVariation;
	float minNu, maxNu, nuVariation;
	int minIterations, maxIterations, iterationsVariation;

	int triesPerLearning;	// Defines how many time the script will let the lemming try the current map with his current learning
	std::vector<int> specialIterationsNumber;	// In case you want some special number of iterations that doesn't come in the above range

	std::vector<std::string> mapPool;	// The maps on which to iterate
};

#endif;