//
// Created by David Johnson on 4/18/17.
//

#include "Agent.h"

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

    // TODO:  These should come from beliefs database
    map<string, int> params;
    params["myTempo"] = 90;
    params["blfTempo"] = 45;

    // Example Callback for Goals
    function<void(bool result, const Goal& g)> tempoCallback = [this, params](bool result, const Goal& g){

        if (result){
            cout << "Goal Meet" << endl;
        }
        else{
            cout << "Goal Not Meet: " << g << endl;
//            patch.sendTempo(params.at("myTempo")/2);
        }
    };

    // Setup Some Goals
    goals.push_back( Goal({"myTempo", "==", "blfTempo"}, tempoCallback) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "/", "2"}, tempoCallback) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "/", "3"}, tempoCallback) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "/", "4"}, tempoCallback) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "*", "2"}, tempoCallback) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "*", "3"}, tempoCallback) );
    goals.push_back( Goal({"myTempo", "==", "blfTempo", "*", "4"}, tempoCallback) );

    // TODO: Fix for "And" Goals and "Or" Goals
    // TODO: Add to Interpreter
    vector<Goal> options;

    for (Goal g : goals){
        if ( !g.evaluate(params) ){
            options.push_back(g);
        }
    }

    Goal g = *(select_randomly(options.begin(), options.end()));

    g.runFunction(params);

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

