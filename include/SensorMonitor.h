//
// Created by David Johnson on 4/24/17.
//

#ifndef MATO_PROXIMITYMONITOR_H
#define MATO_PROXIMITYMONITOR_H

#include "common.h"
#include "OscListener.h"

class SensorMonitor{

public:

    SensorMonitor(int oscPort) : oscIn(oscPort) {}

    virtual void start(){
        oscIn.start();
    }

    virtual void addFunction(string name, callbackFunction func){
        oscIn.onReceive(name, func);
    }


private:

    OscListener oscIn;

};


#endif //MATO_PROXIMITYMONITOR_H
