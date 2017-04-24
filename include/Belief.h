//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BELIEF_H
#define MATO_BELIEF_H

#include "common.h"

class Belief;
typedef shared_ptr<map<string, shared_ptr<Belief>>> Beliefs;

class Belief{

public:
    Belief() : paramName(0), value(0){};
    Belief(string name, float value) : paramName(name), value(value) {};

    string  paramName;
    float   value;

    friend ostream& operator<<(ostream& os, const Belief& belief);
};



#endif //MATO_BELIEF_H
