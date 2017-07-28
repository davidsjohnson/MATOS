//
// Created by David Johnson on 7/19/17.
//

#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "common.h"
#include "Agent.h"

TEST_CASE( "Latency Sync Check", "[sync][timeline]" ) {

    string ip0 = "127.0.0.1";
    string ip1 = "127.0.0.1";

    // Agent 0 Parameters
    int agentID0 = 0;
    int oscPort0 = 7007;
    string pdFile0 = "pd/main.pd";
    int n_states0 = 4;
    map<int, pair<string, int>> neighbors0;

    // Agent 1 Parameters
    int agentID1 = 1;
    int oscPort1 = 7001;
    string pdFile1 = "pd/main.pd";
    int n_states1 = 4;
    map<int, pair<string, int>> neighbors1;

    // Add Neighbors
    neighbors0[agentID1] = pair<string, int>(ip1, oscPort1);
    neighbors1[agentID0] = pair<string, int>(ip0, oscPort0);

    //Setup Agents
    shared_ptr<Agent> agent1 = make_shared<Agent>(agentID1, neighbors1, pdFile1, false, oscPort1, n_states1);
    agent1->start();

    shared_ptr<Agent> agent0 = make_shared<Agent>(agentID0, neighbors0, pdFile0, true, oscPort0, n_states0);
    agent0->start();

    REQUIRE(agent0->getAgentID() == 0);
    REQUIRE(agent1->getAgentID() == 1);

    REQUIRE(agent0->isMaster());
    REQUIRE(!agent1->isMaster());

    sleep(100);

    agent0->stop();
    agent1->stop();

    REQUIRE(true);

}