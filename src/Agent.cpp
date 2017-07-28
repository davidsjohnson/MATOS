//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"

Agent::Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, bool master, int oscPort, const int num_states) :
        id(agentID), oscPort(oscPort), patchFile(pdFile), master(master),
        neighbors(neighbors), patch(pdFile), n_states(num_states),
        goals(make_shared<vector<Goal, allocator<Goal>>>()),
        agentMonitor(make_shared<AgentMonitor>(oscPort)), sensorMonitor(oscPort+1000),
        beliefs(make_shared<map<string, shared_ptr<Belief>>>()), bdi(),
        behaviors(), timeline(agentID, patch, master)
{


    //TODO: Research MultiAgent Decision Making processes - voting techniques

    // #############
    // Setup OscOuts for all neighbors
    // #############
    for (pair<int, pair<string, int>> n : neighbors){
        shared_ptr<OscSender> o = make_shared<OscSender>(n.second.second, n.second.first);
        oscOuts.push_back(o);

        timeline.addNeighbor(n.first, o);
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
                patch.sendTempo( tempo);        // TODO:  Can remove this...
                timeline.setTempo(tempo);
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
            patch.sendState((int(params["myState"])+1)%n_states);
        }

    };

    Goal pdStateGoal1({ "myState", ">=", "maxWorldState", "-", "2", "or",
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
    // Set up Sensor Input (Touch OSC sent through Wekinator is the "Sensor" for now)
    // #############

    // Create a callback function for the sensor
    CallbackFunction volumeCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
            float value = arg->AsFloat()*127;  // for now only one argument for proximity as values come from TouchOSC for demo...
            patch.sendParameters("volume-fromCpp", {value});
            cout << "New Volume=" << value << endl;
        }
        catch(exception e){
            cout << "Error Adding Proximity Belief: " << e.what() << endl;
        }
    };


    CallbackFunction tempoCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
            float value = arg->AsFloat();  // for now only one argument for proximity as values come from TouchOSC for demo...
            patch.sendTempo(value*240);
            timeline.setTempo(value*240);
            cout << "New Tempo="<< value*240 << endl;
        }
        catch(exception e){
            cout << "Error Adding Proximity Belief: " << e.what() << endl;
        }
    };


    CallbackFunction padCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
            float value = arg->AsFloat();  // for now only one argument for proximity as values come from TouchOSC for demo...

            if (value >= 0) {                       // HACK: For TouchOSC buttons...
                patch.sendState(int(value));
                cout << "Changing State:" << value << endl;
            }
        }
        catch(exception e){
            cout << "Error Adding Pad Belief: " << e.what() << endl;
        }
    };

    CallbackFunction startCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
//            float value = arg->AsFloat();  // for now only one argument for proximity as values come from TouchOSC for demo...
            patch.sendStart();
        }
        catch(exception e){
            cout << "Error Adding Start Belief: " << e.what() << endl;
        }
    };

        // Create the sensor
    sensorMonitor.addFunction("/volume/.*", volumeCallback);    // Add the callbacks
    sensorMonitor.addFunction("/tempo/.*", tempoCallback);
    sensorMonitor.addFunction("/start/.*", startCallback);
    sensorMonitor.addFunction("/pad/.*", padCallback);

    //Initialize the Patch
    patch.init(this);

}

Agent::~Agent() {
    stop();
}


void Agent::start() {

    for (auto b : behaviors) {
        b->init(beliefs, agentMonitor);
    }


    srand(time(NULL));
    int randTempo = rand() % 50 + 50;

    // Start the sensor
    sensorMonitor.start();
    bdi.start();
    agentMonitor->start();

    timeline.init(agentMonitor);
    timeline.syncAllAgents();
    timeline.setTempo(randTempo);
    timeline.start();
    cout << "Agent " << id << " Ready and Started" << endl;

}



void Agent::stop() {

    timeline.stop();
    sensorMonitor.stop();
    agentMonitor->stop();
    bdi.stop();
    patch.closePatch();

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

bool Agent::isMaster() {
    return master;
}
