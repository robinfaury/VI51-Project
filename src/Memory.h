#ifndef MEMORY_H_
#define MEMORY_H_

#include <vector>

class Memory
{
private:
std::vector<std::pair<int,int>> memory;

public :
	void reset();

    void addPosition(std::pair<int,int> position);
    void addPosition(int x, int y);

    bool visited(std::pair<int,int> position);  // Returns true if lemming visited given position
    bool visited(int x, int y);                 // Returns true if lemming visited given position
};

#endif
