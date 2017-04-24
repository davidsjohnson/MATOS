//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BDI_INTERPRETER_H
#define MATO_BDI_INTERPRETER_H

#include "Belief.h"
#include "Goal.h"
#include "ActionGenerator.h"

class Interpreter {

public:
    Interpreter() : m_beliefs(), m_goals() {}
    Interpreter(map<string, shared_ptr<Belief>>& beliefs, vector<Goal>& goals);

    ~Interpreter();

//    static void update(map<string, shared_ptr<Belief>>& beliefs, vector<Goal>& goals, map<string, int> blfParams);
    void update();
    void start();
    void run();

    void setBeliefs(const map<string, shared_ptr<Belief>> &m_beliefs);

    void setGoals(const vector<Goal, allocator<Goal>> &m_goals);

private:
    map<string, shared_ptr<Belief>> m_beliefs;
    vector<Goal>                    m_goals;
    map<string, int>                m_blfParams;

    thread                          bdi;

};

#endif //MATO_BDI_INTERPRETER_H
