//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_AGENT_H
#define MATO_AGENT_H

#include "common.h"

#include "PdPatch.h"
#include "Interpreter.h"
#include "Belief.h"
#include "OscMonitor.h"
#include "OscSender.h"

class Agent {

public:

    Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, const int& oscPort);

    void    start();
    int     getAgentID(){return id;}

private:
    int                 id;
    int                 oscPort;
    string              patchFile;
    map<int, pair<string, int>> neighbors;

    PdPatch                 patch;
    OscMonitor              oscMonitor;
    vector<shared_ptr<OscSender>>       oscOuts;

    // BDI objects
    Interpreter                     bdi;
    vector<Goal>                    goals;
    map<string, shared_ptr<Belief>> beliefs;

    friend ostream& operator<<(ostream& os, const Agent& agent);
};


#endif //MATO_AGENT_H
