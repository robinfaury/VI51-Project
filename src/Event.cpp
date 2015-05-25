#include "Event.h"

Event::Event(EVENT_TYPE type, int x1, int y1, int x2, int y2) : m_type(type), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
{

}

void Event::getPosition1(int& x1, int& y1)
{
    x1 = m_x1;
    y1 = m_y1;
}

    void Event::getPosition2(int& x2, int& y2)
    {
        x2 = m_x2;
        y2 = m_y2;
    }

    void Event::getPositions(int& x1, int& y1, int& x2, int& y2)
    {
        x1 = m_x1;
        y1 = m_y1;
        x2 = m_x2;
        y2 = m_y2;
    }

    EVENT_TYPE Event::getType()
    {
        return m_type;
    }

    void Event::setPosition1(int x1, int y1)
    {
        m_x1 = x1;
        m_y1 = y1;
    }

    void Event::setPositions(int x1, int y1, int x2, int y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = y2;
        m_y2 = y2;
    }
