//
// Created by David Johnson on 4/18/17.
//

#include "Interpreter.h"

#include <random>

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



Interpreter::Interpreter(map<string, shared_ptr<Belief>>& beliefs, vector<Goal>& goals) :
    m_beliefs(beliefs), m_goals(goals)
{
    m_blfParams["myTempo"] = 45;
    m_blfParams["blfTempo"] = 70;
}

void Interpreter::update() {

     //Interpreter Steps Here...

    // Process Beliefs - Creates Params List
    float blfTempoSum = 0;
    int   tempoCnt = 0;
    for (auto beliefPair : m_beliefs){
        Belief b = *beliefPair.second;
        if (b.paramName == "tempo"){
            blfTempoSum+=b.value;
            tempoCnt++;
        }
    }

    if (tempoCnt > 0) {
        m_blfParams["blfTempo"] = (int) blfTempoSum/tempoCnt;
    }
    else{
        m_blfParams["blfTempo"] = m_blfParams["myTempo"];
    }

    // Check Goals against Params List
    vector<Goal> options;
    for (Goal g : m_goals){
        if ( !g.evaluate(m_blfParams) ){
            options.push_back(g);
        }
        else{
            g.action(m_blfParams);
        }
    }

    // Select Actions from Goals and perform Action
    if(options.size() > 0) {
        Goal g = *(select_randomly(options.begin(), options.end()));
        g.action(m_blfParams);
    }

}

void Interpreter::start(){

    // TODO: Implement locking mechanisms for beliefs and goals databases
    bdi = thread(&Interpreter::run, this);
//    bdi.join();
}

Interpreter::~Interpreter() {
    if (bdi.joinable()) bdi.join();
}

void Interpreter::setBeliefs(const map<string, shared_ptr<Belief>> &beliefs) {
    m_beliefs = beliefs;
}

void Interpreter::setGoals(const vector<Goal, allocator<Goal>> &goals) {
    m_goals = goals;
}

void Interpreter::run(){

    while (true){
        update();
        this_thread::sleep_for(chrono::milliseconds(40));
    }

}
