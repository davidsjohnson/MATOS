//
// Created by David Johnson on 4/26/17.
//

#ifndef MATO_TEMPOBEHAVIOR_H
#define MATO_TEMPOBEHAVIOR_H

#include "common.h"

#include "Behavior.h"
#include "Goal.h"
#include "Belief.h"
#include "AgentMonitor.h"


/**
 * A behavior for handling tempo data
 */
class TempoBehavior : public Behavior{

public:
    explicit TempoBehavior(Goal g);


    /**
     * Initializes the behavior to handle OSC Input from neighbors
     *
     * @param beliefs - reference to the agents belief database
     * @param agentMonitor - the monitor that is listening for state changes
     */
    void init(Beliefs beliefs, shared_ptr<AgentMonitor> oscMonitor) override;


    /**
     * Adds the required parameters for the corresponding goal to blfParams
     *
     * @param beliefs - the set of current Agent Beliefs
     * @param blfParams - map containing parameters required for all goals
     */
    void processBeliefs(Beliefs beliefs, map<string, float> &blfParams) override;


    /**
     * generates a list of possible actions for the agent to perform
     *
     * @param blfParams - map of generated parameter values
     * @param actions - a list of actions to perform and if they are required or not
     */
    void generateAction(map<string, float>& blfParams, vector<pair<bool, Goal>>& actionGoals) override;

private:
    Goal    goal;
    Beliefs m_beliefs;

};


#endif //MATO_TEMPOBEHAVIOR_H
