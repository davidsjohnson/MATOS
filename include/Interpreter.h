//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BDI_INTERPRETER_H
#define MATO_BDI_INTERPRETER_H

#include "Belief.h"
#include "Goal.h"


class Interpreter {

public:
    Interpreter() : m_beliefs(), m_goals() {}
    Interpreter(Beliefs beliefs, Goals goals);

    ~Interpreter();

//    static void update(map<string, shared_ptr<Belief>>& beliefs, vector<Goal>& goals, map<string, int> blfParams);
    void update();
    void start();
    void run();

    void setBeliefs(Beliefs m_beliefs);
    void setGoals(Goals m_goals);

private:
    Beliefs             m_beliefs;
    Goals               m_goals;
    map<string, float>    m_blfParams;

    thread              bdi;

};

#endif //MATO_BDI_INTERPRETER_H
