//
// Created by David Johnson on 4/26/17.
//

#ifndef MATO_BEHAVIOR_H
#define MATO_BEHAVIOR_H

#include "common.h"

#include "Goal.h"
#include "Belief.h"
#include "AgentMonitor.h"

class Behavior;

typedef vector<shared_ptr<Behavior>> Behaviors;

/**
 * Interface for BDI behaviors.  Behaviors represent what action an
 * agent should perform for a specified belief (or set of beliefs).  All
 * behaviors should implement this interface.
 */
class Behavior {

public:

    /**
     * Initializes the behavior to handle OSC Input from neighbors
     *
     * @param beliefs - reference to the agents belief database
     * @param agentMonitor - the monitor that is listening for state changes
     */
    virtual void init(Beliefs beliefs, shared_ptr<AgentMonitor> agentMonitor) = 0;

    /**
     * processBeliefs should add the required parameters for the corresponding
     * goal to blfParams
     *
     * @param beliefs - the set of current Agent Beliefs
     * @param blfParams - map containing parameters required for all goals
     */
    virtual void processBeliefs(Beliefs beliefs, map<string, float> &blfParams) = 0;


    /**
     * generates a list of possible actions for the agent to perform
     *
     * @param blfParams - map of generated parameter values
     * @param actions - a list of actions to perform and if they are required or not
     */
    virtual void generateAction(map<string, float>& blfParams, vector< pair<bool, Goal> >& actionGoals) = 0;
};


#endif //MATO_BEHAVIOR_H
