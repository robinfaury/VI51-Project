#ifndef LEARNINGMETHOD_H_
#define LEARNINGMETHOD_H_

/** \class LearningMethod
*   The LearningMethod class is the base class for all the learning algorithms of the the program.
*   It defines the base methods that must be implemented for the simulator to employ them in the simulation.
*/

#include "Agent.h"
#include "World.h"

class LearningMethod
{
protected:
    World* currentWorld; /*!< Pointer to the current world */

	LearningMethod(void);

public:
    LearningMethod(World* world);

    //! Performs the actual learning.
    /**
    * The learn function will be executed once by the Simulator. If it finished correctly, learningComplete will return true.
    * This function can use the World, but can't modify it.
	* returns true if the learning finished correctly
    */
    virtual bool learn() = 0;

    //! Returns true if the learning has finished correctly.
    virtual bool learningComplete() = 0;    // Returns true if learning is complete

    //! Creates and returns an agent of the appropriate type, and setup correctly, to use the learning that has been done.
    /**
    *   When the learning is complete (not before!), this function returns an agent. The agent contains the necessary code to use the performed learning.
    */
    virtual Agent* createAgent() = 0;   // Creates and returns an agent set up to be simulated with what has been learned.

    //! Generates a report of the learning.
    /**
    *   When the learning is complete, this function creates a report containing information about the result, stats, etc.
    */
    virtual std::string generateReport();

	// Loading and saving learning results
	virtual void serialize(pugi::xml_node* node) = 0;
	virtual void unzerialize(pugi::xml_node* node) = 0;


};

#endif
