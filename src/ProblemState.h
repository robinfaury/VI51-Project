#ifndef PROBLEMSTATE_H
#define PROBLEMSTATE_H

/*
 * ProblemState represents one state of the problem. There will be as many instances of this class per problem that there
 * are states per problem. One state stores :
 * - The state of the world
 * - A list of possible actions
 * - An id, that is the index of its place in its containing array
 */

#include <string>
#include <vector>

#define QLEARNING_DEBUG 0

enum DIRECTION
{
	LEFT,
	RIGHT,
	DOWN
};

enum TILE_TYPE
{
	EMPTY_OR_DIGGABLE,
	UNDIGGABLE
};

class ProblemState
{
public:
    ProblemState(int id);
    ~ProblemState();

	int getId();

protected:
	//FIXME : remove that (or use it) if we have enough time
	std::vector<std::string> m_possibleActions;
	const int m_id;

};

#endif // PROBLEMSTATE_H
