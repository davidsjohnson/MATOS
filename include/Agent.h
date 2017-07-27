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
#include "AgentMonitor.h"
#include "OscSender.h"
#include "SensorMonitor.h"
#include "TempoBehavior.h"
#include "PdStateBehavior.h"
#include "SensorMonitor.h"
#include "Timeline.h"

/**
 * Agent is the main class that encapsulates all components of the BDI agent. An agent
 * is initialized with a set of neighbors (a map of IP address and Port pairs), a PD
 * patch that adheres to the MATO API and an OSC Port for incoming messages.
 *
 * An agent is composed of goals, behaviors and beliefs that are processed by the
 * agent's interpreter.  See XXXXX for more details on the BDI architecture of MATO
 *
 * Example:
 *      Agent agent(agentID, neighbors, pdFile, oscPort);
 *      agent.start()
 */
class Agent {

public:

    /**
     * Intializes the agent and creates OSC Out connections to all neighbors
     *
     * @param agentID - A unique agent ID
     * @param neighbors - A map of neighbor connection details <key: AgentID; value: <IP Address, OSC Port>>
     * @param pdFile - the location of the PD patch for sound generation
     * @param oscPort - the port to use for incoming OSC messages
     */
    Agent(int agentID,  map<int, pair<string, int>> neighbors, const string& pdFile, bool master, int oscPort, const int num_states);
    ~Agent();


    /**
     * Starts all components of the Agent, including the interpreter, the PD Patch and the OSC server
     */
    void    start();

    void    stop();


    /**
     * Updates the agent's state value for the given parameter
     *
     * @param paramName
     * @param value
     */
    void    updateState(string paramName, float value);

    /**
     *
     * @return the agents ID
     */
    int     getAgentID(){return id;}

    bool    isMaster();

private:

    // Agent Info
    int                 id;
    int                 oscPort;
    string              patchFile;
    bool                master;

    //Audio Objects
    PdPatch patch;
    int     n_states;
    Timeline timeline;

    //Agent to Agent Communication Objects
    map<int, pair<string, int>>     neighbors;
    vector<shared_ptr<OscSender>>   oscOuts;
    shared_ptr<AgentMonitor>        agentMonitor;

    // Senors
    SensorMonitor                   sensorMonitor;

    // BDI objects
    Interpreter                     bdi;
    Goals                           goals;
    Beliefs                         beliefs;
    vector<shared_ptr<Behavior>>    behaviors;


    /**
     * Updates all neighbors with the value for the given parameter
     *
     * @param paramName
     * @param value
     */
    void    updateNeighbors(string paramName, float value);


    friend ostream& operator<<(ostream& os, const Agent& agent);
};


#endif //MATO_AGENT_H
