//
// Created by David Johnson on 4/26/17.
//

#ifndef MATO_PD_H
#define MATO_PD_H

#include "common.h"
#include "Behavior.h"

class PdStateBehavior : public Behavior {

public:

    PdStateBehavior(Goal g) : goal(g){}

    void init(Beliefs &beliefs, OscMonitor &oscMonitor) override;
    void processBeliefs(const Beliefs beliefs, map<string, float> &blfParams) override;
    void generateAction(map<string, float> &blfParams, vector<pair<bool, Goal> > &actionGoals) override;

private:

    Goal goal;

};

#endif //MATO_PD_H
