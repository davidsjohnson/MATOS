#include "Agent.h"
#include "cxxopts.hpp"

#include <fstream>

using namespace std;


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
            ;

    options.parse(argc, argv);

    // TODO:  Add Error Handling and Help Messages
    int agentID = options["i"].as<int>();
    int oscPort = options["o"].as<int>();
    string pdFile = options["p"].as<string>();
    string nbFile = options["n"].as<string>();

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
    Agent agent(agentID, neighbors, pdFile, oscPort);
    cout << "Started Agent:\n";
    cout << agent << endl;
    agent.start();

    return 0;
}