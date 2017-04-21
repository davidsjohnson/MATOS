//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"

Agent::Agent(int agentID, map<int, string> neighbors, const string& pdFile, const int& oscPort) :
    id(agentID), oscPort(oscPort), patchFile(pdFile),
    neighbors(neighbors), patch(pdFile), bdi(),
    monitor(oscPort), beliefs()
{
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

        beliefs.addBelief(message.AddressPattern(), make_shared<Belief>("tempo", tempo));
        cout << beliefs << endl;
    };

    monitor.addFunction("/tempo/.*", tempoFunc);
}

void Agent::start() {
    patch.sendStart();
    monitor.start();
}


ostream& operator<<(ostream& os, const Agent& agent){

    os << "Agent: " << agent.id << " | Listening on Port: " << agent.oscPort;
    os << "\n\t Playing Patch: " << agent.patchFile;

    return os;
}

