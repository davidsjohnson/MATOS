//
// Created by David Johnson on 4/18/17.
//

#include "Interpreter.h"

#include <random>

extern int g_agentID;

// #############
// Functions for Selecting Random element from iterator
// #############
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}
// #############
// #############


void Interpreter::update() {

    // ####### Process Beliefs - Creates Params List
    for(auto b : m_behaviors){
        b->processBeliefs(m_beliefs, m_blfParams);
    }

    // ####### Generate List of Actions using Params List
    vector<pair<bool, Goal>> actionGoals;
    for(auto b: m_behaviors){
        b->generateAction(m_blfParams, actionGoals);
    }

    // ####### Run all required Actions and generate list of optional parameters
    vector<Goal> options;
    for(auto g : actionGoals){
        if(g.first){
            g.second.action(m_blfParams);
        }
        else{
            options.push_back(g.second);
;        }
    }

    // #######  Select An action to from the optional actions
    if(options.size() > 0) {
        Goal g = *(select_randomly(options.begin(), options.end()));
        g.action(m_blfParams);
    }

}

void Interpreter::start(){
    // TODO: Implement locking mechanisms for beliefs and goals databases
    // TODO: Start Different Thread of Sensor Input
    bdi = thread(&Interpreter::run, this);
    t_printBeliefs = thread(&Interpreter::printBeliefs, this);
}

Interpreter::~Interpreter() {
    if (bdi.joinable()) bdi.join();
    if (t_printBeliefs.joinable()) t_printBeliefs.join();
}

void Interpreter::setBeliefs(Beliefs beliefs) {
    m_beliefs = beliefs;
}

void Interpreter::setGoals(Goals goals) {
    m_goals = goals;
}

void Interpreter::run(){

    while (true){
        update();
        this_thread::sleep_for(chrono::milliseconds(500));
    }

}

void Interpreter::printBeliefs() {

    while(true) {
        cout << "\nCurrent Beliefs:" << endl;
        for (auto bPair : *m_beliefs) {
            cout << "\t" << bPair.first << ": " << *bPair.second << endl;
        }
        cout << endl;
        this_thread::sleep_for(chrono::milliseconds(30000));
    }
}
