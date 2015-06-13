#include "ScriptManager.h"

ScriptManager::ScriptManager(LearningManager* learningManager, World* world, std::string& currentLevelPath) : learningManager(learningManager), world(world), currentLevelPath(currentLevelPath), agent(NULL)
{
	
}

ScriptManager::~ScriptManager()
{
	this->clearScriptManager();
}

void ScriptManager::clearScriptManager()
{
	this->clearMapPool();
	this->clearSpecialIterationNumbers();

	if (this->agent != NULL)
	{
		delete(this->agent);
		this->agent = NULL;
	}
}

void ScriptManager::launchScript(std::string name)	// the main script. Launch it only when ready. (And be prepared for A LONG RUNTIME)
{
	this->qLearning = (QLearning*)this->learningManager->getLearningMethod(LEARNING_TYPE::QLEARNING);
	if (qLearning == NULL)
	{
		std::cout << "ERROR : ScriptManager::launchScript : couldn't convert the learning method into a QLearning one" << std::endl;
	}

	// Computing total script savefile name
	std::string completePath = sourcesPath;
	completePath = completePath + resPath + scriptResultsPath + name + extensionPath;

	// creating save document
	pugi::xml_document doc;
	pugi::xml_node scriptNode = doc.append_child("Script");
	scriptNode.append_attribute("name").set_value(name.data());


	// Initial display
	std::cout << std::endl << std::endl;
	std::cout << "	!! Starting script " << name << " !!" <<  std::endl;
	std::cout << std::endl << std::endl;

	std::cout << "Parameters : " << std::endl;
	//Tries per learning
	std::cout << " - numberOfTriesPerLearning : " << this->triesPerLearning << std::endl;
	std::cout << std::endl;

	// iterations
	std::cout << " - minIterations			: " << this->minIterations << std::endl;
	std::cout << " - maxIterations			: " << this->maxIterations << std::endl;
	std::cout << " - iterationsVariation	: " << this->iterationsVariation << std::endl;
	std::cout << std::endl;

	// alpha
	std::cout << " - minAlpha		: " << this->minAlpha << std::endl;
	std::cout << " - maxAlpha		: " << this->maxAlpha << std::endl;
	std::cout << " - alphaVariation	: " << this->alphaVariation << std::endl;
	std::cout << std::endl;

	//gamma
	std::cout << " - minGamma		: " << this->minGamma << std::endl;
	std::cout << " - maxGamma		: " << this->maxGamma << std::endl;
	std::cout << " - gammaVariation	: " << this->gammaVariation << std::endl;
	std::cout << std::endl;

	// rho
	std::cout << " - minRho			: " << this->minRho << std::endl;
	std::cout << " - maxRho			: " << this->maxRho << std::endl;
	std::cout << " - rhoVariation	: " << this->rhoVariation << std::endl;
	std::cout << std::endl;

	// nu
	std::cout << " - minNu			: " << this->minNu << std::endl;
	std::cout << " - maxNu			: " << this->maxNu << std::endl;
	std::cout << " - nuVariation	: " << this->nuVariation << std::endl;
	std::cout << std::endl;

	// map pool size
	std::cout << " - mapPool size : " << this->mapPool.size() << std::endl;

	// Variables
	float numberOfSuccesses = 0;
	float averageNumberOfSuccesses = 0;
	float percentageOfSuccess = 0;
	float totalPercentageOfSuccess = 0;
	float averageSuccessLength = 0;
	float totalAverageSuccessLength = 0;

	pugi::xml_node resultsNode = scriptNode.append_child("Results");
	pugi::xml_node tempResultNode;

	std::cout << std::endl << "Beginning script" << std::endl << std::endl;

	// LET THE SCRIPTING BEGIN
	for (std::vector<std::string>::iterator mapIterator = this->mapPool.begin(); mapIterator != this->mapPool.end(); ++mapIterator)	// Iterating for every map
	{
		// Iterating for every iteration number
		for (int iterations = this->minIterations; iterations <= this->maxIterations;)	// purposely leaving the last section blank : we'll increment i ourselves
		{
			// Iterating for every alpha value
			for (float alpha = this->minAlpha; alpha <= this->maxAlpha;)
			{
				// Iterating for every gamma value
				for (float gamma = this->minGamma; gamma <= this->maxGamma;)
				{
					// Iterating for every rho value
					for (float rho = this->minRho; rho <= this->maxRho;)
					{
						// Iterating for every nu value
						for (float nu = this->minNu; nu <= this->maxNu;)
						{
							std::cout << "Learning : map(" << *mapIterator << "), iterations(" << iterations << "), alpha(" << alpha << "), gamma(" << gamma << "), rho(" << rho << "), nu(" << nu << ")" << std::endl;

							// Learning for the map
							this->resetMap(*mapIterator);
							this->qLearning->learn(iterations, alpha, gamma, rho, nu);

							if (SCRIPTMANAGER_DEBUG)
								std::cout << "ScriptManager::LaunchScript : Starting tests for current iteration" << std::endl;

							// Doing the tests
							this->resetMap(this->currentLevelPath);
							this->resetAgent();

							this->testingCurrentScriptingState(this->triesPerLearning, this->world->getSize()*this->world->getSize(), percentageOfSuccess, averageSuccessLength, numberOfSuccesses);

							// Computing results
							if (SCRIPTMANAGER_DEBUG)
								std::cout << "ScriptManager::LaunchScript : Serializing tests results" << std::endl;
							averageNumberOfSuccesses += numberOfSuccesses;
							averageNumberOfSuccesses = averageNumberOfSuccesses / 2;

							totalPercentageOfSuccess += percentageOfSuccess;
							totalPercentageOfSuccess = totalPercentageOfSuccess / 2;

							totalAverageSuccessLength += averageSuccessLength;
							totalAverageSuccessLength = totalAverageSuccessLength / 2;

							// Serializing current results
							tempResultNode = resultsNode.append_child("ToBeChanged");
							this->serializeCurrentResult(&tempResultNode, alpha, gamma, rho, nu, iterations, percentageOfSuccess, averageSuccessLength, numberOfSuccesses);
							
							if (SCRIPTMANAGER_DEBUG)
								std::cout << "ScriptManager::LaunchScript : Preparing new iteration" << std::endl;

							// stepping rho
							if (nu == this->maxNu)
								++nu;
							else
							{
								nu += this->nuVariation;
								if (nu > this->maxNu)
									nu = this->maxNu;
							}
						}
						// stepping rho
						if (rho == this->maxRho)
							++rho;
						else
						{
							rho += this->rhoVariation;
							if (rho > this->maxRho)
								rho = this->maxRho;
						}
					}
					// stepping gamma
					if (gamma == this->maxGamma)
						++gamma;
					else
					{
						gamma += this->gammaVariation;
						if (gamma > this->maxGamma)
							gamma = this->maxGamma;
					}
				}
				// stepping alpha
				if (alpha == this->maxAlpha)
					++alpha;
				else
				{
					alpha += this->alphaVariation;
					if (alpha > this->maxAlpha)
						alpha = this->maxAlpha;
				}
			}
			// stepping i
			if (iterations == this->maxIterations)
				++iterations;	// Nothing. stepping out of the loop
			else
			{
				// Incrementing i
				iterations += this->iterationsVariation;
				if (iterations > this->maxIterations)	// if i steps over this->maxIterations, we make sure we'll actually do it
					iterations = this->maxIterations;
			}
		}
		std::cout << "Finished iterating for map " << *mapIterator << std::endl;
		cout << "Saving result : " << completePath.data() << " : " << doc.save_file(completePath.data()) << endl;
	}

	// Saving final results
	std::cout << "Script completed! " << std::endl;

	pugi::xml_node reportNode = scriptNode.append_child("Report");
	reportNode.append_attribute("averageNumberOfSuccesses").set_value(averageNumberOfSuccesses);
	reportNode.append_attribute("totalAveragePercentageOfSuccess").set_value(totalPercentageOfSuccess);
	reportNode.append_attribute("totalAverageSuccessLength").set_value(totalPercentageOfSuccess);

	cout << "Saving result : " << completePath.data() << " : " << doc.save_file(completePath.data()) << endl;
}

// For the current map, tests the lemming's ability to complete the map
void ScriptManager::testingCurrentScriptingState(int triesPerScriptingState, int maxLength, float& percentageOfSuccess, float& averageSuccessLength, float& numberOfSuccesses)
{
	std::vector<int> succesLength;
	numberOfSuccesses = 0;

	// Iterating for every try
	for (int tries = 0; tries < triesPerScriptingState; ++tries)
	{
		this->resetMap(this->currentLevelPath);
		this->resetAgent();

		// Trying the map
		for (int walkLength = 1; walkLength <= maxLength; ++walkLength)
		{
			// Agent life
			this->world->setPerceptions();
			this->agent->live();

			// Computing results
			this->world->collectInfluences();
			this->world->resolveInfluences();

			if (SCRIPTMANAGER_DEBUG)
				std::cout << "Current Lemming Pos : " << this->world->getBodies()->at(0)->getPosition().at(0) << "," << this->world->getBodies()->at(0)->getPosition().at(1) << std::endl;

			if (this->world->lemmingSuccess())	// Lemming succeeded
			{
				// incrementing successes, and collecting walkLength
				++numberOfSuccesses;
				succesLength.push_back(walkLength);
				break;
			}
			// If it didn't work, try again
		}
	}

	percentageOfSuccess = (numberOfSuccesses / triesPerScriptingState) * 100;
	averageSuccessLength = 0;

	for (std::vector<int>::iterator it = succesLength.begin(); it != succesLength.end(); ++it)
	{
		averageSuccessLength += *it;
	}
	if (succesLength.empty())
		averageSuccessLength = 0;
	else
		averageSuccessLength = averageSuccessLength / succesLength.size();
}

void ScriptManager::resetAgent()
{
	if (this->agent != NULL)
	{
		delete (this->agent);
	}
		

	
	this->agent = this->learningManager->getAgent(LEARNING_TYPE::QLEARNING);
	if (SCRIPTMANAGER_DEBUG && this->agent == NULL)
		std::cout << "agent is null "<< std::endl;
	else if (SCRIPTMANAGER_DEBUG )
		std::cout << "agent isn't null " << std::endl;
	this->agent->linkBody(this->world->getBodies()->at(0));
}

// Configure parameters
void ScriptManager::addMapToPool(std::string mapName)
{
	// Checking that the map is valid
	if (!this->world->loadLevel(mapName))
	{
		std::cout << "ERROR : ScriptManager::addMapToPool : couldn't add " << mapName << " to pool, invalid name" << std::endl;
		return;
	}

	//Then, check if it doesn't exist already in the map pool
	for (std::vector<std::string>::iterator it = this->mapPool.begin(); it != this->mapPool.end(); ++it)
	{
		// Found it : return
		if ((*it) == mapName)
		{
			std::cout << "ERROR : ScriptManager::addMapToPool : " << mapName << " is already part of the map pool" << std::endl;
			return;
		}
	}

	//Finally, add it
	this->mapPool.push_back(mapName);
}

void ScriptManager::clearMapPool()
{
	this->mapPool.clear();
}

void ScriptManager::addSpecialIterationNumber(int specialIterationNumber)
{
	for (std::vector<int>::iterator it = this->specialIterationsNumber.begin(); it != this->specialIterationsNumber.end(); ++it)
	{
		if ((*it) == specialIterationNumber)
			return;
	}

	this->specialIterationsNumber.push_back(specialIterationNumber);
}

void ScriptManager::clearSpecialIterationNumbers()
{
	this->specialIterationsNumber.clear();
}

void ScriptManager::setAlpha(float minAlpha, float maxAlpha, float alphaVariation)
{
	this->minAlpha = minAlpha;
	this->maxAlpha = maxAlpha;
	this->alphaVariation = alphaVariation;
}

void ScriptManager::setGamma(float minGamma, float maxGamma, float gammaVariation)
{
	this->minGamma = minGamma;
	this->maxGamma = maxGamma;
	this->gammaVariation = gammaVariation;
}

void ScriptManager::setRho(float minRho, float maxRho, float rhoVariation)
{
	this->minRho = minRho;
	this->maxRho = maxRho;
	this->rhoVariation = rhoVariation;
}

void ScriptManager::setNu(float minNu, float maxNu, float nuVariation)
{
	this->minNu = minNu;
	this->maxNu = maxNu;
	this->nuVariation = nuVariation;
}

void ScriptManager::setIterations(int minIterations, int maxIterations, int iterationsVariation)
{
	this->minIterations = minIterations;
	this->maxIterations = maxIterations;
	this->iterationsVariation = iterationsVariation;
}

void ScriptManager::setTriesPerLearning(int triesPerLearning)
{
	this->triesPerLearning = triesPerLearning;
}

void ScriptManager::serializeCurrentResult(pugi::xml_node* node, float alpha, float gamma, float rho, float nu, int iterations, float percentageOfSuccess, float averageSuccessLength, float numberOfSuccesses)
{
	if (node == NULL)
	{
		std::cout << "ERROR : ScriptManager::serializeCurrentResult : given node is NULL" << std::endl;
		return;
	}

	std::string nodeName = this->currentLevelPath;
	nodeName += "#" + std::to_string(iterations) + "iterations";
	node->set_name(nodeName.data());

	node->append_attribute("alpha").set_value(alpha);
	node->append_attribute("gamma").set_value(gamma);
	node->append_attribute("rho").set_value(rho);
	node->append_attribute("nu").set_value(nu);
	node->append_attribute("iterations").set_value(iterations);
	node->append_attribute("percentageOfSuccess").set_value(percentageOfSuccess);
	node->append_attribute("averageSuccessLength").set_value(averageSuccessLength);
	node->append_attribute("numberOfSuccesses").set_value(numberOfSuccesses);
}

void ScriptManager::resetMap(std::string levelPath)
{
	if (!this->world->loadLevel(levelPath))
	{
		std::cout << "ERROR : ScriptManager::resetMap : World could not load map. Switching to default map..." << std::endl;
		this->world->loadLevel(this->currentLevelPath);
	}
	else
	{
		this->currentLevelPath = levelPath;
	}
}