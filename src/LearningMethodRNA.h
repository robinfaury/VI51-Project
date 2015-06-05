#ifndef LEARNINGMETHODRNA_H_
#define LEARNINGMETHODRNA_H_

/** \class LearningMethod
*   The LearningMethod class is the base class for all the learning algorithms of the the program.
*   It defines the base methods that must be implemented for the simulator to employ them in the simulation.
*/

#include "LearningMethod.h"
#include "NeuronalLemming.h"

class LearningMethodRNA : public LearningMethod
{
protected:
    World* currentWorld; /*!< Pointer to the current world */
	
	bool learningFinish;

	NeuronalNetworkMonocouche* ArtificialInteligence;

	LearningMethodRNA(void);

public:
    LearningMethodRNA(World* world, std::string filename);

    //! Performs the actual learning.
    /**
    * The learn function will be executed once by the Simulator. If it finished correctly, learningComplete will return true.
    * This function can use the World, but can't modify it.
	* returns true if the learning finished correctly
    */
    virtual bool learn();

    //! Returns true if the learning has finished correctly.
    virtual bool learningComplete();    // Returns true if learning is complete

    //! Creates and returns an agent of the appropriate type, and setup correctly, to use the learning that has been done.
    /**
    *   When the learning is complete (not before!), this function returns an agent. The agent contains the necessary code to use the performed learning.
    */
    virtual Agent* createAgent();   // Creates and returns an agent set up to be simulated with what has been learned.

    //! Generates a report of the learning.
    /**
    *   When the learning is complete, this function creates a report containing information about the result, stats, etc.
    */
    virtual std::string generateReport();

	~LearningMethodRNA();
};

#endif
