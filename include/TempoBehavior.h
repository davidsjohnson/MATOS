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

private:c
    Goal goal;

};


#endif //MATO_TEMPOBEHAVIOR_H
// ###### Tempo Goal and Behavior
ActionFunction tempoAction;
tempoAction = [this](bool result, const Goal& g, map<string, float>& params){

    if (!result){
        cout << "Tempo Goal Not Met: " << g << endl;
        float tempo = params.at("worldTempo");
        patch.sendTempo( tempo);
    }
};

Goal tempoGoal = Goal({"myTempo", "==", "worldTempo",           "or",
                       "myTempo", "==", "worldTempo", "*", "2", "or",
                       "myTempo", "==", "worldTempo", "*", "3", "or",
                       "myTempo", "==", "worldTempo", "*", "4", "or",
                       "myTempo", "==", "worldTempo", "/", "2", "or",
                       "myTempo", "==", "worldTempo", "/", "3", "or",
                       "myTempo", "==", "worldTempo", "/", "4"
                      }, tempoAction);

behaviors.push_back(make_shared<TempoBehavior>(tempoGoal));
// ###### End Tempo Goal and Behavior