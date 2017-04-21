//
// Created by David Johnson on 4/18/17.
//

#include "Interpreter.h"

void Interpreter::registerBeliefs(Beliefs beliefs) {
    m_beliefs = beliefs;
}

void Interpreter::registerGoals(Goals goals) {
    m_goals = goals;
}

void Interpreter::registerOptionGenerator(OptionGenerator og) {
    m_og = og;
}

void Interpreter::update() {

    vector<Option> options;
    m_og.generateOptions(options);
}
