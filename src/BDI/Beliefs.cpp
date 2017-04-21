//
// Created by David Johnson on 4/18/17.
//

#include "Beliefs.h"

void Beliefs::addBelief(string beliefID, string parameterName, float value) {
    shared_ptr<Belief> b = make_shared<Belief>(parameterName, value);
    beliefs[beliefID] = b;
}


void Beliefs::addBelief(string beliefID, shared_ptr<Belief> belief) {
    beliefs[beliefID] = belief;
}


ostream& operator<<(ostream& os, const Beliefs& beliefs){
    os << "Beliefs: { ";
    for (auto p : beliefs.beliefs){
        os << p.first << " : " << *p.second << ", ";
    }
    os << "}";

    return os;
}


ostream& operator<<(ostream& os, const Belief& belief){
    os << "(" << belief.name << ", " << belief.value << ")";
    return os;
}

