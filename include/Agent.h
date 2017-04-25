//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_AGENT_H
#define MATO_AGENT_H

#include "common.h"

#include "PdPatch.h"
#include "Interpreter.h"
#include "Belief.h"
#include "Goal.h"
#include "OscMonitor.h"
#include "OscSender.h"
#include "ProximityMonitor.h"

class Agent {

public:

    Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, const int& oscPort);

    void    start();
    int     getAgentID(){return id;}

    // Update the state of the current agent
    void    updateState(string paramName, float value);

private:
    int                 id;
    int                 oscPort;
    string              patchFile;


    //Output/Audio Objects
    PdPatch patch;

    //TODO: Sensor AgentComm Objects Go Here
    //  like accelerometer data and microphone data

    //Agent AgentComm Objects
    map<int, pair<string, int>>     neighbors;
    vector<shared_ptr<OscSender>>   oscOuts;
    OscMonitor                      oscMonitor;
    ProximityMonitor                proximityMonitor;

    // BDI objects
    Interpreter bdi;
    Goals       goals;
    Beliefs     beliefs;

    // Update all neighbors
    void    updateNeighbors(string paramName, float value);

    friend ostream& operator<<(ostream& os, const Agent& agent);
};


#endif //MATO_AGENT_H
