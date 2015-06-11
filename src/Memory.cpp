#include "Memory.h"

void Memory::reset()
{
    this->memory.clear();
}

void Memory::addPosition(std::pair<int,int> position)
{
    if (!visited(position))
        this->memory.push_back(position);
}

void Memory::addPosition(int x, int y)
{
    std::pair<int,int> position(x,y);
    this->addPosition(position);
}

// Returns true if lemming visited given position
bool Memory::visited(std::pair<int,int> position)
{
    if (this->memory.find(position) != this->end())
        return true;
    return false;
}

// Returns true if lemming visited given position
bool Memory::visited(int x, int y)
{
    std::pair<int,int> position(x,y);
    return this->visited(position);
}
