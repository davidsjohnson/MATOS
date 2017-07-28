//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BELIEF_H
#define MATO_BELIEF_H

#include "common.h"

class Belief;

/**
 * The agent's database of all beliefs representing the agent's
 * knowledge of its environment
 */
typedef shared_ptr<map<string, shared_ptr<Belief>>> Beliefs;

/**
 * A Belief is a represented by a parameter name and an associated value
 */
class Belief{

public:

    Belief(string name, float value) : paramName(std::move(name)), value(value) {};

    const string    paramName;
    float           value;

    friend ostream& operator<<(ostream& os, const Belief& belief);
};



#endif //MATO_BELIEF_H
