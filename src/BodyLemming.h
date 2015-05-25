#ifndef BODYLEMMING_H_
#define BODYLEMMING_H_

#include "Body.h"

class BodyLemming : public Body
{
private:


public :
    BodyLemming(Semantic* type);

    virtual Perception* getPerception();
};

#endif
