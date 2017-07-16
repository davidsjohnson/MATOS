//
// Created by David Johnson on 4/24/17.
//

#ifndef MATO_PROXIMITYMONITOR_H
#define MATO_PROXIMITYMONITOR_H

#include "common.h"
#include "OscListener.h"


/**
 * Listens for communication from sensors
 */
class SensorMonitor{

public:

    /**
     * Initializes the listener on the specified OSC Port
     *
     * @param oscPort
     */
    SensorMonitor(int oscPort) : oscIn(oscPort) {}


    /**
     * Starts the monitor
     */
    virtual void start(){
        oscIn.start();
    }


    /**
     * Registers a callback function to the given address pattern
     *
     * @param paramAddr - address pattern of the recieved parameter.
     *      Example: /tempo/.* represents tempo from all agent IDs
     * @param callback - the function to run if pattern is matched, should be of type:
     *      typedef std::function<void(const osc::ReceivedMessage& message)> callbackFunction;
     */
    virtual void addFunction(string name, CallbackFunction func){
        oscIn.onReceive(name, func);
    }


private:

    OscListener oscIn;

};


#endif //MATO_PROXIMITYMONITOR_H
