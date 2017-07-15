//
// Created by David Johnson on 4/26/17.
//

#ifndef MATO_PD_H
#define MATO_PD_H

#include "common.h"
#include "Behavior.h"

/**
 * A behavior for handling state information from pure data
 */
class PdStateBehavior : public Behavior {

public:

    PdStateBehavior(Goal g) : goal(g){}

    /**
     * Initializes the behavior to handle OSC Input from neighbors
     *
     * @param beliefs - reference to the agents belief database
     * @param agentMonitor - the monitor that is listening for state changes
     */
    void init(Beliefs &beliefs, AgentMonitor &oscMonitor) override;


    /**
     * Adds the required parameters for the corresponding goal to blfParams
     *
     * @param beliefs - the set of current Agent Beliefs
     * @param blfParams - map containing parameters required for all goals
     */
    void processBeliefs(const Beliefs beliefs, map<string, float> &blfParams) override;



    /**
     * generates a list of possible actions for the agent to perform
     *
     * @param blfParams - map of generated parameter values
     * @param actions - a list of actions to perform and if they are required or not
     */
    void generateAction(map<string, float> &blfParams, vector<pair<bool, Goal> > &actionGoals) override;

private:

    Goal goal;

};

#endif //MATO_PD_H
