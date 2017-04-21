#include <unistd.h>
#include "Agent.h"
#include "cxxopts.hpp"

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

    int agentID = options["i"].as<int>();
    int oscPort = options["o"].as<int>();
    string pdFile = options["p"].as<string>();
    string nbFile = options["n"].as<string>();

    // TODO:  Implement loading neighbors file
    map<int, string> neighbors;

    Agent agent(agentID, neighbors, pdFile, oscPort);
    cout << "Starting Agent:\n";
    cout << agent << endl;
    agent.start();

    return 0;
}