//
// Created by David Johnson on 4/18/17.
//

#include "Interpreter.h"

#include  <random>
#include  <iterator>


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

Interpreter::Interpreter(map<string, shared_ptr<Belief>> beliefs, vector<Goal> goals) :
    m_beliefs(beliefs), m_goals(goals)
{
}

void Interpreter::update() {

    // Interpreter Steps Here...
    // Process Beliefs - Creates Params List
    // Check Goals against Params List
    // Select Actions from Goals
    // Perform Actions

    for (auto beliefPair : m_beliefs){
        cout << "Belief - " << beliefPair.first << ":  " << *beliefPair.second <<endl;
    }

    cout << endl;

    for (auto goal : m_goals){
        cout << "Goal - " << goal.name << ": " << goal << endl;
    }

    vector<Goal> options;

    m_blfParams["myTempo"] = 45;
    m_blfParams["blfTempo"] = 75;

    for (Goal g : m_goals){
        if ( !g.evaluate(m_blfParams) ){
            options.push_back(g);
        }
        else{
            g.action(m_blfParams);
        }
    }

    if(options.size() > 0) {
        Goal g = *(select_randomly(options.begin(), options.end()));
        g.action(m_blfParams);
    }
}

void Interpreter::start(){
    update();
}
