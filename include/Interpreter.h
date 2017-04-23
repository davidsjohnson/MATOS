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
    Interpreter(map<string, shared_ptr<Belief>> beliefs, vector<Goal> goals);

    void update();
    void start();

private:
    map<string, shared_ptr<Belief>> m_beliefs;
    vector<Goal>                    m_goals;
    map<string, int>                m_blfParams;

};

#endif //MATO_BDI_INTERPRETER_H
