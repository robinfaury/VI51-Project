#ifndef EVENT_H_
#define EVENT_H_

/*
*   When an event occurs in the world, the World will create an event to be passed to the GraphicsEngine
*/

enum EVENT_TYPE
{
    MOVE,REMOVE
};

class Event
{
public :
    Event(EVENT_TYPE type, int x1, int y1, int x2 = -1, int y2 = -1);

    void getPosition1(int& x1, int& y1);
    void getPosition2(int& x2, int& y2);
    void getPositions(int& x1, int& y1, int& x2, int& y2);

    EVENT_TYPE getType();

    void setPosition1(int x1, int y1);
    void setPositions(int x1, int y1, int x2, int y2);

private :
    EVENT_TYPE m_type;
    int m_x1,m_y1,m_x2,m_y2;
};

#endif
