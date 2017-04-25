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



Interpreter::Interpreter(Beliefs beliefs, Goals goals) :
    m_beliefs(beliefs), m_goals(goals)
{
}

void Interpreter::update() {

     //Interpreter Steps Here...

    // Process Beliefs - Creates Params List
    float blfTempoSum = 0;
    int   tempoCnt = 0;

    vector<float> tempos;
    vector<float> states;
    vector<float> volumes;

    for (auto beliefPair : *m_beliefs){
        Belief b = *beliefPair.second;
        if (b.paramName == "tempo"){
            if (beliefPair.first == "/tempo/"+to_string(g_agentID)){
                m_blfParams["myTempo"] = b.value;
            }
            else {
                tempos.push_back(b.value);
            }
        }
        else if (b.paramName == "state"){
            if (beliefPair.first == "/state/"+to_string(g_agentID)){
                m_blfParams["myState"] = b.value;
            }
            else{
                states.push_back(b.value);
            }
        }
        else if (b.paramName == "stateChgTime"){
            if (beliefPair.first == "/stateChgTime/"+to_string(g_agentID)){
                m_blfParams["stateChgTime"] = b.value;
            }
        }
        else if (b.paramName == "myProximity"){
            m_blfParams["myProximity"] = b.value;
        }
        else if (b.paramName == "proximity"){
            m_blfParams["proximity"] = b.value;
        }
        else if (b.paramName == "volume"){
            if ( beliefPair.first == "/volume/"+to_string(g_agentID) ) {
                m_blfParams["myVolume"] = b.value;
            }
            else{
                volumes.push_back(b.value);
            }
        }
    }
    m_blfParams["currentTime"] = time(NULL) % 2592000; // moding to reduce size of value to fit in float

    // Process Tempos to find param value
    if (tempos.size() > 0) {
        // TODO:  Find better method for tempo estimation
        float avg = accumulate(tempos.begin(), tempos.end(), 0) / tempos.size();
        m_blfParams["blfTempo"] = avg;
    }
    else{
        m_blfParams["blfTempo"] = m_blfParams["myTempo"];
    }

    // Process States to find param values
    if (states.size() > 0) {
        auto maxState = max_element(states.begin(), states.end());
        m_blfParams["blfState"] = *maxState;
    }
    else{
        float state = m_blfParams["myState"];
        m_blfParams["blfState"] = state;
    }

    // Process Volumes to find param values
    if (volumes.size() > 0) {
        float avg = accumulate(volumes.begin(), volumes.end(), 0) / volumes.size();
        m_blfParams["worldVolume"] = avg;
    }
    else{
        float volume = m_blfParams["myVolume"];
        m_blfParams["worldVolume"] = volume;
    }

    // Check Goals against Params List
    vector<Goal> options;
    for (Goal g : *m_goals){
        try {
            if ( !g.evaluate(m_blfParams) ){
                options.push_back(g);
            }
            else{
                g.action(m_blfParams);
            }
        }
        catch(MissingParameterException e){
//            cout <<"Error: Unable to Eval Goal: " << e.what()  << endl;
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
}

Interpreter::~Interpreter() {
    if (bdi.joinable()) bdi.join();
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
        this_thread::sleep_for(chrono::milliseconds(33));
    }

}
