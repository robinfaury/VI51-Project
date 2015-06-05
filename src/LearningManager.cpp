#include "LearningManager.h"


LearningManager::LearningManager()
{
    //TODO : init every learning method
}

LearningManager::~LearningManager()
{
    //TODO : remove every learning method
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
    //TODO: check casting
    return this->methods.at((int)type);
}

Agent* LearningManager::getAgent(LEARNING_TYPE pe)  // When learning is complete, create and return an agent of appropriate type.
{
    if (this->methods.at(type)->learningComplete())
        return this->methods.at(type)->createAgent();
}
