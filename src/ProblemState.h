#ifndef PROBLEMSTATE_H
#define PROBLEMSTATE_H

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

/*
 * ProblemState represents one state of the problem. There will be as many instances of this class per problem that there
 * are states per problem. One state stores :
 * - The state of the world
 * - A list of possible actions
 * - An id, that is the index of its place in its containing array
 */

#include <string>
#include <vector>


class ProblemState
{
public:
    ProblemState(int id);
    ~ProblemState();

	int getId();

protected:
	std::vector<std::string> m_possibleActions;
	const int m_id;

};

#endif // PROBLEMSTATE_H
