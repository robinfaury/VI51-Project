#ifndef SEMANTIC_H_
#define SEMANTIC_H_

enum SEMANTIC
{
	T_DIRT = 0,
	T_ROCK,
	T_EXIT,
	T_BOUND,	// World limit. Can't be manually placed or removed (except in editor mode)
	B_LEMMING
};

const int SEMANTIC_SIZE = 5;

#endif
