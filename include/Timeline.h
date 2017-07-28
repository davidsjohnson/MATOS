//
// Created by David Johnson on 7/18/17.
//

#ifndef MATO_TIMELINE_H
#define MATO_TIMELINE_H

#include "common.h"
#include <tuple>

#include "OscSender.h"
#include "AgentMonitor.h"
#include "PdPatch.h"


// TODO: Unit Testing

class Timeline {

public:

    Timeline(int agentID, PdPatch &patch, bool master);
    void init(shared_ptr<AgentMonitor> agentMonitor);


private:

    int                         agentID;
    shared_ptr<AgentMonitor>    m_agentMonitor;
    PdPatch                     &patch;

    float m_tempo;
    bool m_master;
    float tickTime;
    bool running;

    condition_variable cv;
    mutex cv_m;
    std::unique_lock<std::mutex> lk;

    thread t_ticks;

    mutex mut_l;
    mutex mut_n;
    mutex mut_d;

    map<int, shared_ptr<OscSender>>         m_neighbors;
    map<int, int>                           m_latencies;          // TODO: sorted maps??
    vector<pair<int, int>>                  m_latenciesSorted;
    map<int, tuple<int, clock_t, clock_t>>  m_syncDetails;


    /*!
     * Should remove neighbors when no response received after XX time
     *
     * @param id
     */
    void syncAgent(int id);

    void broadcastTick();
    void sendTick(shared_ptr<OscSender> oscOut, int delayTime);


public:

    float getTempo() const;
    void setTempo(float tempo);

    bool isMaster() const;
    void makeMaster(bool master=true);

    void addNeighbor(int id, shared_ptr<OscSender> oscOut);
    void syncAllAgents();

    void start();
    void run();
    void stop();


    /*!
     * Callback function to handle receiving a beat from another agent
     *
     * @param id
     */
    void receiveTick(int id);


    /*!
     * Callback function to handle receiving a sync return message from another agent
     *
     * @param id
     */
    void receiveSyncReturn(int id);


    /*!
     * Callback function to handle receiving a sync return message from another agent.
     * Sends a syncreturn to the originating agent.
     *
     * @param id
     */
    void receiveSync(int id);

};


#endif //MATO_TIMELINE_H
