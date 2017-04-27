//
// Created by David Johnson on 4/26/17.
//

#ifndef MATO_BEHAVIOR_H
#define MATO_BEHAVIOR_H

#include "common.h"

#include "Goal.h"
#include "Belief.h"
#include "OscMonitor.h"

class Behavior {

public:
    virtual void init(Beliefs& beliefs, OscMonitor& oscMonitor) = 0;
    virtual void processBeliefs(const Beliefs beliefs, map<string, float>& blfParams) = 0;
    virtual void generateAction( map<string, float>& blfParams, vector< pair<bool, Goal> >& actionGoals) = 0;
};


#endif //MATO_BEHAVIOR_H
