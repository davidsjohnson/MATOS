//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BDI_INTERPRETER_H
#define MATO_BDI_INTERPRETER_H

#include "Belief.h"
#include "Goal.h"
#include "Behavior.h"
#include "TempoBehavior.h"


/**
 * The Interpreter is the core of the BDI model. Running an update loop at 2 frames
 * per second.  Each iteration of update processes the agent's beliefs, generating
 * actions based on the beliefs and agent goals and then performing those actions.
 * After an interpreter is created the agent should then register the belief, goal
 * and behavior databases before starting the agent.
 *      Example:
 *              Interpreter bdi;
 *              bdi.setBeliefs(beliefs)
 *              bdi.setGoals(goals)
 *              bdi.setBehaviors(behaviors)
 *              bdi.start()
 */
class Interpreter {

public:
    Interpreter() : m_behaviors(), m_beliefs(), m_goals() {}
    ~Interpreter();

    /**
     * starts the interpreter
     */
    void start();


    /**
     * registers the agent's belief database
     * @param beliefs - the agent's belief database
     */
    void setBeliefs(Beliefs beliefs);


    /**
     * registers the agent's goal database
     * @param goals - the agent's goal database
     */
    void setGoals(Goals goals);


    /**
     * registers the agent's behavior database
     * @param behaviors - the agents behavior database
     */
    void setBehaviors(vector<shared_ptr<Behavior>> behaviors){m_behaviors = behaviors;}

private:

    vector<shared_ptr<Behavior>>    m_behaviors;
    Beliefs             m_beliefs;
    Goals               m_goals;
    map<string, float>  m_blfParams;

    thread              bdi;
    thread              t_printBeliefs;

    void update();
    void run();
    void printBeliefs();

};

#endif //MATO_BDI_INTERPRETER_H
