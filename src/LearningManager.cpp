#include "LearningManager.h"


LearningManager::LearningManager(World* world) : world(world)
{
	this->init(this->world);

}


LearningManager::~LearningManager()
{
	clearLearningManager();
}

void LearningManager::clearLearningManager()
{
	for (std::vector<LearningMethod*>::iterator it = this->methods.begin(); it != this->methods.end(); ++it)
	{
		delete (*it);
		*it = NULL;
	}
	this->methods.clear();
}

void LearningManager::init(World* world)
{
	if (world != NULL)
	{
		// For each learning method, create it iin this->learningMethods and set corresponding bool to false
		this->methods.push_back(new QLearning(world));
		std::string filePath = sourcesPath;
		filePath += "res/learn.txt";
		this->methods.push_back(new LearningMethodRNA(world, filePath));
	}
	else
	{
		std::cout << "ERROR : LearningManager::init : can't initialise for NULL world" << std::endl;
	}
}

void LearningManager::launchLearning()
{
	launchLearning(LEARNING_TYPE::QLEARNING);
	launchLearning(LEARNING_TYPE::NEURALNETWORK);
}

void LearningManager::launchLearning(LEARNING_TYPE type)
{
	if (this->methods.at(type)->learn())
	{
		std::cout << "Learning with method " << type << " complete" << std::endl;
	}
	else
	{
		std::cout << "ERROR: LearningManager::launchLearning : Learning with method " << type << " didn't execute correctly" << std::endl;
	}
}

void LearningManager::displayReports()
{
    //TODO : display reports from all methods
}

// Sets current world
void LearningManager::setWorld(World* currentWorld)
{
    this->world = currentWorld;
}

World* LearningManager::getWorld()  // gets current world
{
    return this->world;
}


LearningMethod* LearningManager::getMethod(LEARNING_TYPE type)    // Returns a pointer to current learning method
{
    return this->methods.at(type);
}

Agent* LearningManager::getAgent(LEARNING_TYPE type)  // When learning is complete, create and return an agent of appropriate type.
{
    if (this->methods.at(type)->learningComplete())
        return this->methods.at(type)->createAgent();

	return NULL;
}

//Load / save learning result
void LearningManager::serialize(LEARNING_TYPE type, pugi::xml_node* node)
{
	if (this->methods.at(type)->learningComplete())
	{
		this->methods.at(type)->serialize(node);
	}
}

void LearningManager::unserialize(LEARNING_TYPE type, pugi::xml_node* node)
{
	this->methods.at(type)->unzerialize(node);
}

LearningMethod* LearningManager::getLearningMethod(LEARNING_TYPE type)
{
	return this->methods.at(type);
}