//
// Created by David Johnson on 4/26/17.
//

#ifndef MATO_TEMPOBEHAVIOR_H
#define MATO_TEMPOBEHAVIOR_H

#include "common.h"

#include "Behavior.h"
#include "Goal.h"
#include "Belief.h"
#include "OscMonitor.h"

class TempoBehavior : public Behavior{

public:
    TempoBehavior(Goal g);

    virtual void init(Beliefs& beliefs, OscMonitor& oscMonitor) override;

    virtual void processBeliefs(const Beliefs beliefs, map<string, float>& blfParams) override;
    virtual void generateAction(map<string, float>& blfParams, vector<pair<bool, Goal>>& actionGoals) override;

private:
    Goal goal;

};


#endif //MATO_TEMPOBEHAVIOR_H
