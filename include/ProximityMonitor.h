//
// Created by David Johnson on 4/24/17.
//

#ifndef MATO_PROXIMITYMONITOR_H
#define MATO_PROXIMITYMONITOR_H

#include "common.h"
#include "Monitor.h"
#include "OscListener.h"

class ProximityMonitor : public Monitor{

public:

    ProximityMonitor(int oscPort);

    void start() override;

    void addFunction(string name, callbackFunction func) override;

private:

    OscListener oscIn;

};


#endif //MATO_PROXIMITYMONITOR_H
