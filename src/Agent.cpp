//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"

Agent::Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, const int& oscPort) :
        id(agentID), oscPort(oscPort), patchFile(pdFile),
        neighbors(neighbors), patch(pdFile), goals(make_shared<vector<Goal, allocator<Goal>>>()),
        oscMonitor(oscPort), proximityMonitor(oscPort+1000), beliefs(make_shared<map<string, shared_ptr<Belief>>>()), bdi()
{

    //TODO: Just Me or All updates
    //TODO: Research MultiAgent Decision Making processes - voting techniques
    //TODO: Update Commenting
    //TODO: Possible options for interactively changing the state if you don't like it - sensors
    //TODO: Sliding scale for state change time distances...


    // #############
    // Setup OscOuts for all neighbors
    // #############
    for (pair<int, pair<string, int>> n : neighbors){
        shared_ptr<OscSender> o = make_shared<OscSender>(n.second.second, n.second.first);
        oscOuts.push_back(o);
    }

    // #############
    // Adding Callback Function for Receiving Tempo AgentComm Messages
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
    // Adding Callback Function for Receiving State AgentComm Messages
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
    // Configuring Proximity Sensor
    // #############
    // TODO:  Change the method for sensors - shouldn't be a goal
    // ######## Adding OSC Callback For handling proximity messages
    callbackFunction proximityCallback = [&](const osc::ReceivedMessage& message){

        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();

        float value;
        if (arg->IsInt32()) {
            value = (float) arg->AsInt32();
        } else if (arg->IsInt64()) {
            value = (float) arg->AsInt64();
        } else if (arg->IsFloat()) {
            value = arg->AsFloat();
        } else {
            throw osc::WrongArgumentTypeException();
        }

        // Add (or update) a belief for Tempo message from a neighbor
        (*beliefs)[message.AddressPattern()] = make_shared<Belief>("proximity", value);

        cout << "Current Beliefs:" << endl;
        for (auto p : *beliefs) {
            cout << "Belief: " << p.first << " : " << (*p.second) << endl;
        }

    };
    proximityMonitor.addFunction("/proximity/.*", proximityCallback);
    proximityMonitor.start();

    // ######## Action Function for proximity
    function<void(bool result, const Goal& g, map<string, float>& params)> proximityAction;
    proximityAction = [this](bool result, const Goal& g, map<string, float>& params){
        if (result){
            cout << "Proximity Goal Met: " << g << endl;
            float volume = params.at("proximity");
            patch.sendParameters("volume-fromCpp", {volume});
            updateState("myProximity", volume);
        }
    };

    // ######## Adding Proximity Goal and Action
    goals->push_back( Goal({"proximity", ">", "1", "and",
                            "proximity", "<", "127", "and",
                            "proximity", "!=", "myProximity"
                           }, proximityAction) );

    updateState("myProximity", 0);  // initialize state
    // Proximity Sensor Configured
    // #############


    // #############
    // Configuring Volume Rules
    // #############

    callbackFunction volumeCallback = [&](const osc::ReceivedMessage& message){

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
        (*beliefs)[message.AddressPattern()] = make_shared<Belief>("volume", tempo);

        cout << "Current Beliefs:" << endl;
        for (auto p : *beliefs) {
            cout << "Belief: " << p.first << " : " << (*p.second) << endl;
        }

    };
    oscMonitor.addFunction("/volume/.*", volumeCallback);

    function<void(bool result, const Goal& g, map<string, float>& params)> volumeAction;
    volumeAction = [this](bool result, const Goal& g, map<string, float>& p){

        if (!result){
            cout << "Volume Goal Not Met: " << g << endl;
            float volume = p.at("worldVolume");
            patch.sendParameters("volume-fromCpp", {volume});
        }
    };


    // Adding a Goal for Tempo and assigning the tempo callback
    goals->push_back( Goal({ "myVolume", "-", "5", "<=", "worldVolume", "and",
                             "myVolume", "+", "5", ">=", "worldVolume"
                           }, volumeAction) );
    // Volume Rule Configured
    // #############


    // #############
    // Adding Action for a Tempo Goal  TODO: Change all instances to "Action"
    // #############
    function<void(bool result, const Goal& g, map<string, float>& params)> tempoCallback;
    tempoCallback = [this](bool result, const Goal& g, map<string, float>& p){

            if (!result){
                cout << "Tempo Goal Not Met: " << g << endl;
                float tempo = p.at("blfTempo");
                patch.sendTempo( tempo);
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
        }
    };

    srand (time(NULL));

    // TODO: This should change at every state change....
    int minTime = rand() % 20 + 25;
    int maxTime = rand() % 25 + 40;


//    int maxTime = 15;

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


}


void Agent::start() {
    int randTempo = rand() % 30 + 90;
    patch.sendStart(randTempo);
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
