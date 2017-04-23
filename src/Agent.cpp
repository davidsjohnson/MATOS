//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"


Agent::Agent(int agentID, map<int, string> neighbors, const string& pdFile, const int& oscPort) :
    id(agentID), oscPort(oscPort), patchFile(pdFile),
    neighbors(neighbors), patch(pdFile), goals(),
    oscMonitor(oscPort), beliefs(), bdi()
{

    // Adding Callback Function for Receiving Tempo Input Messages
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
        beliefs[message.AddressPattern()] = make_shared<Belief>("tempo", tempo);
    };

    oscMonitor.addFunction("/tempo/.*", tempoFunc);

    // Example Callback for Goal
    ResultCallback tempoCallback = [this](bool result, const Goal& g, const map<string, int>& p){

        if (result){
            cout << "Goal Meet" << endl;
        }
        else{
            cout << "Tempo Goal Not Meet: " << g << endl;
            int tempo = p.at("blfTempo");
            patch.sendTempo((float) tempo);
        }
    };

    // Setup Some Goals
    goals.push_back( Goal({"myTempo", "==", "blfTempo",           "or",
                           "myTempo", "==", "blfTempo", "*", "2", "or",
                           "myTempo", "==", "blfTempo", "*", "3", "or",
                           "myTempo", "==", "blfTempo", "*", "4", "or",
                           "myTempo", "==", "blfTempo", "/", "2", "or",
                           "myTempo", "==", "blfTempo", "/", "3", "or",
                           "myTempo", "==", "blfTempo", "/", "4"
                          }, tempoCallback) );

    string idStr = "/tempo/" + to_string(id);
    beliefs[idStr] = make_shared<Belief>("tempo", 45);

    bdi = Interpreter(beliefs, goals);
}

void Agent::start() {
    patch.sendStart();
    bdi.start();
    oscMonitor.start();
}


ostream& operator<<(ostream& os, const Agent& agent){

    os << "Agent: " << agent.id << " | Listening on Port: " << agent.oscPort;
    os << "\n\t Playing Patch: " << agent.patchFile;

    return os;
}

