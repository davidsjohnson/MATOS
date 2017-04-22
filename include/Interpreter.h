//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BDI_INTERPRETER_H
#define MATO_BDI_INTERPRETER_H

#include "Beliefs.h"
#include "Goal.h"
#include "ActionGenerator.h"

class Interpreter {

public:
    Interpreter();

    void registerBeliefs(Beliefs beliefs);
    void registerGoals(vector<Goal> goals);
    void registerOptionGenerator(OptionGenerator og);

    void update();

private:
    Beliefs         m_beliefs;
    vector<Goal>    m_goals;
    OptionGenerator m_og;

};


#endif //MATO_BDI_INTERPRETER_H
