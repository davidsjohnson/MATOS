//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"

Agent::Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, const int& oscPort) :
        id(agentID), oscPort(oscPort), patchFile(pdFile),
        neighbors(neighbors), patch(pdFile), goals(make_shared<vector<Goal, allocator<Goal>>>()),
        oscMonitor(oscPort), proximityMonitor(oscPort+1000),
        beliefs(make_shared<map<string, shared_ptr<Belief>>>()), bdi(),
        behaviors()
{

    //TODO: Just Me or All updates
    //TODO: Research MultiAgent Decision Making processes - voting techniques
    //TODO: Update Commenting
    //TODO: Possible options for interactively changing the state if you don't like it - sensors
    //TODO: Sliding scale for state change time distances...

    //TODO: Implement Granular Synthsis Patches
    //TODO:    User proximity influences grain size
    //TODO: Implement Audio Input and Recording
    //TODO:     Recordings are used for grains
    //TODO:     Grain length is influenced by...
    //TODO:     Other parameters for agent selection?

    // #############
    // Setup OscOuts for all neighbors
    // #############
    for (pair<int, pair<string, int>> n : neighbors){
        shared_ptr<OscSender> o = make_shared<OscSender>(n.second.second, n.second.first);
        oscOuts.push_back(o);
    }


    // ###### Tempo Goal and Behavior
    ActionFunction tempoAction;
    tempoAction = [this](bool result, const Goal& g, map<string, float>& params){
            if (!result){
                cout << "Tempo Goal Not Met: " << g << endl;

                srand(time(NULL));
                int rnd = rand() % 2 + 1;
                float worldTempo = params.at("worldTempo");
                float tempo;
                if (worldTempo <= 65){
                    tempo = worldTempo*(rnd);
                }
                else{
                    tempo = worldTempo/rnd;
                }
                patch.sendTempo( tempo);
            }
        };

    Goal tempoGoal = Goal({"myTempo", "~=", "worldTempo",           "or",
                           "myTempo", "~=", "worldTempo", "*", "2", "or",
                           "myTempo", "~=", "worldTempo", "*", "3", "or",
                           "myTempo", "~=", "worldTempo", "*", "4", "or",
                           "myTempo", "~=", "worldTempo", "/", "2", "or",
                           "myTempo", "~=", "worldTempo", "/", "3", "or",
                           "myTempo", "~=", "worldTempo", "/", "4"
                          }, tempoAction);

    behaviors.push_back(make_shared<TempoBehavior>(tempoGoal));
    // ###### End Tempo Goal and Behavior


    // ###### Pd State Goals and Behavior
    ActionFunction pdStateAction = [this](bool result, const Goal& g, map<string, float>& params){

        if (!result){
            cout << "State Goal Not Met: " << g << endl;
            patch.sendNextState();
        }

    };

    Goal pdStateGoal1({ "myState", ">=", "maxWorldState", "-", "3", "or",
                             "currentTime", "-", "stateChgTime", "<", "rand(25, 45)"
                           }, pdStateAction);

    Goal pdStateGoal2({ "currentTime", "-", "stateChgTime", "<", "rand(40, 65)"
                           }, pdStateAction);

    behaviors.push_back(make_shared<PdStateBehavior>(pdStateGoal1));
    behaviors.push_back(make_shared<PdStateBehavior>(pdStateGoal2));
    // ###### End PD State Goals and Behavior



    // #############
    // Initialize Interpreter with belief, goal and behavior databases
    // #############
    bdi.setBeliefs(beliefs);
    bdi.setGoals(goals);
    bdi.setBehaviors(behaviors);


    // #############
    // Set up Sensor Input
    // #############

    // Create a callback function for the sensor
    callbackFunction proximitySensorCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
            float value = arg->AsFloat();  // for now only one argument for proximity as values come from TouchOSC for demo...
            patch.sendParameters("volume-fromCpp", {value});
        }
        catch(exception e){
            cout << "Error Adding Proximity Belief: " << e.what() << endl;
        }
    };

        // Create the sensor
    proximityMonitor.addFunction("/proximity/.*", proximitySensorCallback);                  // Add the callback
    proximityMonitor.start();                                                                // Start the sensor

    //Initialize the Patch
    patch.init(this);

}


void Agent::start() {

    for (auto b : behaviors){
        b->init(beliefs, oscMonitor);
    }

    srand(time(NULL));
    int randTempo = rand() % 30 + 90;
    patch.sendStart(randTempo);

    bdi.start();
    oscMonitor.start();
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
