//
// Created by David Johnson on 4/24/17.
//

#include "ProximityMonitor.h"

ProximityMonitor::ProximityMonitor(int oscPort) : Monitor(),
        oscIn(oscPort)
{
}

void ProximityMonitor::start() {
    oscIn.start();
}

void ProximityMonitor::addFunction(string name, callbackFunction func) {
    oscIn.onReceive(name, func);
}
