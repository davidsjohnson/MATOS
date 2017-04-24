//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"


Agent::Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, const int& oscPort) :
        id(agentID), oscPort(oscPort), patchFile(pdFile),
        neighbors(neighbors), patch(pdFile), goals(),
        oscMonitor(oscPort), beliefs(), bdi()
{

    // Setup OscOuts for all neighbors
    for (pair<int, pair<string, int>> n : neighbors){
        shared_ptr<OscSender> o = make_shared<OscSender>(n.second.second, n.second.first);
        oscOuts.push_back(o);
    }

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
    function<void(bool result, const Goal& g, map<string, int>& params)> tempoCallback;
    tempoCallback = [this](bool result, const Goal& g, map<string, int>& p){

            if (result){
//                cout << "Goal Meet" << endl;
            }
            else{
                cout << "Tempo Goal Not Meet: " << g << endl;
                int tempo = p.at("blfTempo");
                patch.sendTempo((float) tempo);
                p["myTempo"] = tempo;
                beliefs["/tempo/"+to_string(id)]->value = tempo;

                for (auto o : oscOuts){
                    o->sendOsc("/tempo/"+to_string(id), tempo);
                }
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
    beliefs["/tempo/1"] = make_shared<Belief>("tempo", 45);
    beliefs[idStr] = make_shared<Belief>("tempo", 70);

    bdi = Interpreter(beliefs, goals);
}

void Agent::start() {
    patch.sendStart();
    bdi.start();
    oscMonitor.start(); // TODO: Currently Blocking
}


ostream& operator<<(ostream& os, const Agent& agent){

    os << "Agent: " << agent.id << " | Listening on Port: " << agent.oscPort;
    os << "\n\t Playing Patch: " << agent.patchFile;

    return os;
}

