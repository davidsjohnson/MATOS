#include "Agent.h"
#include "cxxopts.hpp"
#include "common.h"

using namespace std;

int g_agentID;   // TODO:  Find better options for this

int main(int argc, char* argv[]) {

    // TODO:  All - Add commenting.

    // Parsing Command Line Arguments
    cxxopts::Options options(argv[0], " - Multi Agent Tangible Objects");

    int defaultPort = 7000;
    string defaultPatchFile = "pd/main.pd";
    options.add_options()
            ("i,id", "Unique Agent ID", cxxopts::value<int>(), "ID")
            ("o,oscport", "Port for OSC Messages", cxxopts::value<int>(defaultPort), "OSC Port")
            ("p,patchfile", "Path to Pure Data Patch", cxxopts::value<string>(defaultPatchFile), "PD Patch Location")
            ("n,neighborsfile", "Path to the neighbor file", cxxopts::value<string>(), "Neighbors File Location")
            ("s,numberstates", "The number of states available for the patch", cxxopts::value<int>(), "Number of PD States")
            ("m,master", "Indicates the agent is the master of all neighbors", cxxopts::value<bool>(), "Master Status")
            ;

    options.parse(argc, argv);

    // TODO:  Add Error Handling and Help Messages
    int agentID = options["i"].as<int>();
    int oscPort = options["o"].as<int>();
    string pdFile = options["p"].as<string>();
    string nbFile = options["n"].as<string>();
    int n_states = options["s"].as<int>();
    bool master = options["m"].as<bool>();

    // Loading neighbors file
    map<int, pair<string, int>> neighbors;
    if (nbFile.empty()) {
        cout << "No Neighbors Found" << endl;
    }
    else{
        ifstream inFile(nbFile.c_str());
        if(! inFile){
            cerr << "Unable to open file: " << nbFile << " - exiting." << endl;
            exit(1);
        }

        while(inFile){
            string line;
            getline(inFile, line);

            istringstream iss(line);
            try {
                string nID;
                string nIP;
                string nPort;

                iss >> nID;
                iss >> nIP;
                iss >> nPort;

                neighbors[stoi(nID)] = pair<string, int>(nIP, stoi(nPort));
            }
            catch(exception e){
                cout << "Error Parsing Neighbors File: " << e.what() << endl;
            }

        }
    }

    for (auto n : neighbors){
        cout << "Neighbor ID: " << n.first << " OSC: " << n.second.first << " " << n.second.second << endl;
    }

    // Starting Agent with Parameters from Command Line
    g_agentID = agentID;
    Agent agent(agentID, neighbors, pdFile, master, oscPort, n_states);
    if (master)
        cout << "Started Agent as Master:\n";
    else
        cout << "Started Agent:\n";
    cout << agent << endl;

    agent.start();

    pause();

    return 0;
}