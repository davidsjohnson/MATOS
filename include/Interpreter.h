//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BDI_INTERPRETER_H
#define MATO_BDI_INTERPRETER_H

#include <condition_variable>
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
    Interpreter() : m_behaviors(), m_beliefs(), m_goals(), running(false),
                    cv(), cv2(), cv_m(), cv_m2(), lk(cv_m), lk2(cv_m2)
                    {}
    ~Interpreter();

    /**
     * starts the interpreter
     */
    void start();


    void stop();


    /**
     * registers the agent's belief database
     * @param beliefs - the agent's belief database
     */
    void setBeliefs(shared_ptr<map<string, shared_ptr<Belief>>> beliefs);


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
    shared_ptr<map<string, shared_ptr<Belief>>>             m_beliefs;
    Goals               m_goals;
    map<string, float>  m_blfParams;

    thread              bdi;
    thread              t_printBeliefs;
    bool                running;

    condition_variable cv;
    condition_variable cv2;
    mutex cv_m;
    mutex cv_m2;
    std::unique_lock<std::mutex> lk;
    std::unique_lock<std::mutex> lk2;

    void update();
    void run();
    void printBeliefs();

};

#endif //MATO_BDI_INTERPRETER_H
