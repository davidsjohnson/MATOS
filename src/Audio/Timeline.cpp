//
// Created by David Johnson on 7/18/17.
//

#include "Timeline.h"

Timeline::Timeline(int agentID, PdPatch &patch, bool master) :
        agentID(agentID), patch(patch),
        m_master(master), m_agentMonitor(nullptr), running(false),
        cv(), cv_m(), lk(cv_m)
{
    setTempo(120);
}


void Timeline::init(shared_ptr<AgentMonitor> agentMonitor){

    m_agentMonitor = agentMonitor;

    // Create a callback function sending a syncreturn the syncing
    CallbackFunction syncCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
            int id = arg->AsInt32();  // for now only one argument (which is Agent ID)
            receiveSync(id);
        }
        catch(const exception& e){
            cout << "Error: Unable to receive sync message: " << e.what() << endl;
        }
    };
    m_agentMonitor->addFunction("/sync/.*", syncCallback);

    // Create a callback function for the syncing
    CallbackFunction syncReturnCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
            int id = arg->AsInt32();  // for now only one argument (which is Agent ID)
            receiveSyncReturn(id);
        }
        catch(const exception& e){
            cout << "Error: Unable to receive sync return message: " << e.what() << endl;
        }
    };
    m_agentMonitor->addFunction("/syncreturn/.*", syncReturnCallback);

    // Create a callback function for receiving incoming ticks
    CallbackFunction tickCallback = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        try{
            int id = arg->AsInt32();  // for now only one argument (which is Agent ID)
            receiveTick(id);
        }
        catch(const exception& e){
            cout << "Error: Unable to receive tick message: " << e.what() << endl;
        }
    };
    m_agentMonitor->addFunction("/tick/.*", tickCallback);

}


float Timeline::getTempo() const {
    return m_tempo;
}


void Timeline::setTempo(float tempo) {
    m_tempo = tempo;
    tickTime = 60.f / m_tempo / 4;
}


bool Timeline::isMaster() const {
    return m_master;
}


void Timeline::makeMaster(bool master){
    m_master = master;
}


void Timeline::addNeighbor(int id, shared_ptr<OscSender> oscOut) {

    mut_l.lock();
    m_neighbors[id] = oscOut;
    mut_l.unlock();
}


void Timeline::syncAgent(int id) {

    if (m_master) {

        // Add and initialize agent details if it doesn't exist
        if (m_syncDetails.find(id) == m_syncDetails.end()) {
            m_syncDetails[id] = make_tuple<int, clock_t, float>(0, 0, 0);
        }

        // Send the sync message and update detail
        cout << "Sending Sync Message to Agent: " << id << endl;
        ostringstream oss;
        oss << "/sync/" << agentID;
        m_neighbors[id]->sendOsc(oss.str(), agentID);
        get<1>(m_syncDetails[id]) = clock();
        get<0>(m_syncDetails[id]) += 1;

        // Remove neighbors that haven't received a response in 1 Second
        for (auto it = m_syncDetails.begin(); it != m_syncDetails.end();) {

            if ((clock() - get<1>(it->second)) / CLOCKS_PER_SEC >= 1) {
                cout << "Removing Agent ID: " << get<0>(m_syncDetails[it->first]) << " waiting for: " << (clock() - get<1>(it->second)) / CLOCKS_PER_SEC << endl;
                it = m_syncDetails.erase(it);
            } else {
                it++;
            }
        }
    }

}


void Timeline::syncAllAgents() {

    if (m_master) {
        mut_n.lock();
        for (auto kv : m_neighbors) {
            int id = kv.first;

            mut_d.lock();
            // Start sync for agents not already underway...
            if (m_syncDetails.find(id) == m_syncDetails.end()) {
                syncAgent(id);
            } else {
                // Remove Agents that haven't responded in more than 1 second
                if ((get<1>(m_syncDetails[id]) - clock()) / CLOCKS_PER_SEC >= 1) {
                    m_syncDetails.erase(id);
                }
            }
            mut_d.unlock();
        }
        mut_n.unlock();
    }
}


void Timeline::start() {

    if (m_master) {
        running = true;
        t_ticks = thread(&Timeline::run, this);
    }

}

void Timeline::run() {
    while(running) {
        broadcastTick();
        auto ms = chrono::duration<float>(tickTime);
        cv.wait_for(lk, ms, [&]() { return !running; });
    }
}

void Timeline::stop() {
    running = false;
    if (t_ticks.joinable()) t_ticks.join();
    lk.unlock();
    cv.notify_all();
    cout << "Timeline Stopped" << endl;
}


void Timeline::broadcastTick() {

    // Send a beat for each neighbor
    // delay by difference between latencies
    // each send should be a timed thread
    if (!m_latenciesSorted.empty()) {

        mut_l.lock();
        int maxLatency = m_latenciesSorted[0].second;
        vector<pair<int, int>> temp;
        copy(m_latenciesSorted.begin(), m_latenciesSorted.end(), back_inserter(temp));
        mut_l.unlock();

        for (auto kv : temp) {
            mut_n.lock();
            shared_ptr<OscSender> oscOut = m_neighbors[kv.first];
            mut_n.unlock();

            sendTick(oscOut, maxLatency - kv.second);
        }
    }
    patch.sendBang("-tick"); // send tick to self after all have been sent...
}


void Timeline::sendTick(shared_ptr<OscSender> oscOut, int delayTime) {

    this_thread::sleep_for(chrono::milliseconds(delayTime));
    ostringstream oss;
    oss << "/tick/" << agentID;
    oscOut->sendOsc(oss.str(), agentID);

}


// Incoming OSC Callbacks
void Timeline::receiveSync(int id) {

    mut_n.lock();
    cout << "Returning Sync to Agent: " << id << endl;
    ostringstream oss;
    oss << "/syncreturn/" << agentID;
    m_neighbors[id]->sendOsc(oss.str(), agentID);
    mut_n.unlock();
}


void Timeline::receiveSyncReturn(int id) {

    cout << "Received Sync from Agent: " << id << endl;
    // Sync response returned from agent
    mut_d.lock();

    // Calculate latency
    get<2>(m_syncDetails[id]) += ((clock()-get<1>(m_syncDetails[id])) / 2);  // divide by 2 since we don't want round trip latency

    // 4. Repeat 3 times
    // 5. Save Average
    // 6. Remove agent ID from sync list
    if (get<0>(m_syncDetails[id]) == 3){

        mut_l.lock();

        clock_t avg = get<2>(m_syncDetails[id]) / 3;
        m_latencies[id] = int(float(avg)/CLOCKS_PER_SEC * 1000);  // convert clocks to seconds, then milliseconds
        m_syncDetails.erase(id);
        cout << "Latency for Agent " << id << ": " << m_latencies[id] << endl;

        //Once a new latency is added, sort into vector for faster beat sending
        auto cmpDesc = [](pair<int,int> const & a, pair<int,int> const & b)
        {
            return a.second > b.second;
        };

        m_latenciesSorted.clear();
        copy(m_latencies.begin(), m_latencies.end(), back_inserter(m_latenciesSorted));

        std::sort(m_latenciesSorted.begin(), m_latenciesSorted.end(), cmpDesc);

        mut_l.unlock();
    }
    else{
        syncAgent(id);
    }

    mut_d.unlock();

}


void Timeline::receiveTick(int id) {
    patch.sendBang("-tick");
}
