//
// Created by David Johnson on 4/19/17.
//

#ifndef MATO_OSCMONITOR_H
#define MATO_OSCMONITOR_H

#include "OscListener.h"


class OscMonitor{

public:

    OscMonitor(int oscPort);

    virtual void start();
    virtual void addFunction(string name, callbackFunction func);

private:
    OscListener oscIn;

};


#endif //MATO_OSCMONITOR_H
