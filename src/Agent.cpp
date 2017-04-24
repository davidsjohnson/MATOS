//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"

Agent::Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, const int& oscPort) :
        id(agentID), oscPort(oscPort), patchFile(pdFile),
        neighbors(neighbors), patch(pdFile), goals(make_shared<vector<Goal, allocator<Goal>>>()),
        oscMonitor(oscPort), beliefs(make_shared<map<string, shared_ptr<Belief>>>()), bdi()
{

    // #############
    // Setup OscOuts for all neighbors
    // #############
    for (pair<int, pair<string, int>> n : neighbors){
        shared_ptr<OscSender> o = make_shared<OscSender>(n.second.second, n.second.first);
        oscOuts.push_back(o);
    }

    // #############
    // Adding Callback Function for Receiving Tempo Input Messages
    // #############
    callbackFunction tempoFunc = [&](const osc::ReceivedMessage& message) {

        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();

        float tempo;
        if (arg->IsInt32()) {
            tempo = (float) arg->AsInt32();
        } else if (arg->IsInt64()) {
            tempo = (float) arg->AsInt64();
        } else if (arg->IsFloat()) {
            tempo = arg->AsFloat();
        } else {
            throw osc::WrongArgumentTypeException();
        }

        // Add (or update) a belief for Tempo message from a neighbor
        (*beliefs)[message.AddressPattern()] = make_shared<Belief>("tempo", tempo);

        cout << "Current Beliefs:" << endl;
        for (auto p : *beliefs) {
            cout << "Belief: " << p.first << " : " << (*p.second) << endl;
        }
    };
    oscMonitor.addFunction("/tempo/.*", tempoFunc);

    // #############
    // Adding Callback Function for Receiving State Input Messages
    // #############
    callbackFunction stateFunc = [&](const osc::ReceivedMessage& message) {

        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();

        int state;
        if (arg->IsInt32()) {
            state = arg->AsInt32();
        } else if (arg->IsInt64()) {
            state = arg->AsInt64();
        } else if (arg->IsFloat()) {
            state = arg->AsFloat();
        } else {
            throw osc::WrongArgumentTypeException();
        }

        // Add (or update) a belief for Tempo message from a neighbor
        (*beliefs)[message.AddressPattern()] = make_shared<Belief>("state", state);

        cout << "Current Beliefs:" << endl;
        for (auto p : *beliefs) {
            cout << "Belief: " << p.first << " : " << (*p.second) << endl;
        }
    };
    oscMonitor.addFunction("/state/.*", stateFunc);


    // #############
    // Adding Callback for a Tempo Goal
    // #############
    function<void(bool result, const Goal& g, map<string, float>& params)> tempoCallback;
    tempoCallback = [this](bool result, const Goal& g, map<string, float>& p){

            if (!result){
                cout << "Tempo Goal Not Met: " << g << endl;
                int tempo = p.at("blfTempo");
                patch.sendTempo((float) tempo);
            }
        };


    // Adding a Goal for Tempo and assigning the tempo callback
    goals->push_back( Goal({"myTempo", "==", "blfTempo",           "or",
                            "myTempo", "==", "blfTempo", "*", "2", "or",
                            "myTempo", "==", "blfTempo", "*", "3", "or",
                            "myTempo", "==", "blfTempo", "*", "4", "or",
                            "myTempo", "==", "blfTempo", "/", "2", "or",
                            "myTempo", "==", "blfTempo", "/", "3", "or",
                            "myTempo", "==", "blfTempo", "/", "4"
                          }, tempoCallback) );


    // #############
    // Adding Callback for a State Goal
    // #############
    function<void(bool result, const Goal& g, map<string, float>& params)> stateCallback;
    stateCallback = [this](bool result, const Goal& g, map<string, float>& params){


        if (!result){
            cout << "State Goal Not Met: " << g << endl;
            patch.sendNextState();

            int changeTime = params.at("stateChgTime");
            int currentTime = params.at("currentTime");

        }
    };

    srand (time(NULL));

    int minTime = rand() % 45  + 30;
    int maxTime = rand() % 80  + 60;

    goals->push_back( Goal({ "myState", ">=", "blfState", "-", "3", "or",
                             "currentTime", "-", "stateChgTime", "<", to_string(minTime)
                           }, stateCallback));

    goals->push_back( Goal({ "currentTime", "-", "stateChgTime", "<", to_string(maxTime)
                           }, stateCallback));


    // #############
    // Initialize Interpreter with belief and goal databases
    // #############
    bdi.setBeliefs(beliefs);
    bdi.setGoals(goals);

    patch.init(this);

    int randTempo = rand() % 60 + 45;
    patch.sendTempo(randTempo);
}


void Agent::start() {
    patch.sendStart();
    bdi.start();
    oscMonitor.start(); // TODO: Currently Blocking
}


void  Agent::updateState(string paramName, float value) {

    stringstream ss;
    ss << "/" << paramName << "/" << getAgentID();

    string key = ss.str();

    shared_ptr<Belief> b = make_shared<Belief>(paramName, value);
    (*beliefs)[key] = b;
    updateNeighbors(paramName, value);
}


void Agent::updateNeighbors(string paramName, float value) {

    for (shared_ptr<OscSender> n : oscOuts){

        stringstream ss;
        ss << "/" << paramName << "/" << getAgentID();
        n->sendOsc(ss.str(), value);
    }

}


ostream& operator<<(ostream& os, const Agent& agent){

    os << "Agent: " << agent.id << " | Listening on Port: " << agent.oscPort;
    os << "\n\t Playing Patch: " << agent.patchFile;

    return os;
}
