//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_BDI_INTERPRETER_H
#define MATO_BDI_INTERPRETER_H

#include "Beliefs.h"
#include "Goals.h"
#include "OptionGenerator.h"

class Interpreter {

public:
    void registerBeliefs(Beliefs beliefs);
    void registerGoals(Goals goals);
    void registerOptionGenerator(OptionGenerator og);

    void update();

private:
    Beliefs         m_beliefs;
    Goals           m_goals;
    OptionGenerator m_og;

};


#endif //MATO_BDI_INTERPRETER_H
