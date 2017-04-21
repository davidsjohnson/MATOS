//
// Created by David Johnson on 4/19/17.
//

#ifndef MATO_MONITOR_H
#define MATO_MONITOR_H

#include "common.h"

class Monitor {

public:
    virtual void start() = 0;
    virtual void addFunction(string name, callbackFunction func) = 0;

};


#endif //MATO_MONITOR_H
