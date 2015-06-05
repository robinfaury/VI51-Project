#ifndef LEARNINGMANAGER_H_
#define LEARNINGMANAGER_H_

#include <vector>

#include "QLearning.h"
#include "LearningMethodRNA.h"


/**
*   The LearningManager is tasked to trigger the learning in the application, and manage it.
*/
enum LEARNING_TYPE
{
    QLEARNING = 0,
    NEURALNETWORK
};

class LearningManager
{
    protected:
World* world;
std::vector<LearningMethod*> methods;

    public:
    LearningManager(World* world);
    ~LearningManager();

	void init(World* world);
	void launchLearning();
	void launchLearning(LEARNING_TYPE type);

	void displayReports();

    void setWorld(World* currentWorld); // Sets current world
    World* getWorld();  // gets current world

    LearningMethod* getMethod(LEARNING_TYPE type);    // Returns a pointer to learning method of given type

    Agent* getAgent(LEARNING_TYPE type);  // When learning is complete, create and return an agent of appropriate type.
};

#endif
