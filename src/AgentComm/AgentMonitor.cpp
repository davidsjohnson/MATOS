//
// Created by David Johnson on 4/19/17.
//

#include "AgentMonitor.h"

AgentMonitor::AgentMonitor(int oscPort) : oscIn(oscPort)
{
}


void AgentMonitor::start() {
    oscIn.start();
}


void AgentMonitor::addFunction(string paramAddr, CallbackFunction callback) {
    oscIn.onReceive(paramAddr, callback);
}