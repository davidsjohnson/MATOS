//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"

Agent::Agent(int agentID, map<int, string> neighbors, const string& pdFile, const int& oscPort) :
    id(agentID), oscPort(oscPort), patchFile(pdFile),
    neighbors(neighbors), patch(pdFile), bdi(),
    oscMonitor(oscPort), beliefs()
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
        beliefs.addBelief(message.AddressPattern(), make_shared<Belief>("tempo", tempo));
        cout << beliefs << endl;
    };

    oscMonitor.addFunction("/tempo/.*", tempoFunc);

    // Setup Some Goals
    goals.push_back( Goal({"myTempo", "==", "blfTempo"}) );

    goals.push_back( Goal({"myTempo", "==", "blfTempo", "/", "2"}) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "/", "3"}) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "/", "4"}) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "*", "2"}) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "*", "3"}) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "*", "4"}) );

}

void Agent::start() {
    patch.sendStart();
    oscMonitor.start();
}


ostream& operator<<(ostream& os, const Agent& agent){

    os << "Agent: " << agent.id << " | Listening on Port: " << agent.oscPort;
    os << "\n\t Playing Patch: " << agent.patchFile;

    return os;
}

