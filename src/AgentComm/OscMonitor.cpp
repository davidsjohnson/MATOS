//
// Created by David Johnson on 4/19/17.
//

#include "OscMonitor.h"

OscMonitor::OscMonitor(int oscPort) : Monitor(),
        oscIn(oscPort)
{
}


void OscMonitor::start() {
    oscIn.start();
}


void OscMonitor::addFunction(string name, callbackFunction func) {
    oscIn.onReceive(name, func);
}