//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BELIEFS_H
#define MATO_BELIEFS_H

#include "common.h"

class Belief{

public:
    Belief() : name(0), value(0){};
    Belief(string name, float value) : name(name), value(value) {};

    string  name;
    float   value;

    friend ostream& operator<<(ostream& os, const Belief& belief);
};



class Beliefs{

public:

    void addBelief(string beliefID, string parameterName, float value);
    void addBelief(string beliefID, shared_ptr<Belief> belief);

    friend ostream& operator<<(ostream& os, const Beliefs& beliefs);

private:
    map<string, shared_ptr<Belief>> beliefs;

};


#endif //MATO_BELIEFS_H
