//
// Created by David Johnson on 4/19/17.
//

#ifndef MATO_OSCMONITOR_H
#define MATO_OSCMONITOR_H

#include "OscListener.h"

/**
 * Listens for messages from neighboring agents
 */
class AgentMonitor{

public:

    /**
     * Initializes the listener on the specified OSC Port
     *
     * @param oscPort
     */
    AgentMonitor(int oscPort);


    /**
     * Starts the monitor
     */
    virtual void start();

    /**
    * Stops the monitor
    */
    virtual void stop();


    /**
     * Registers a callback function to the given address pattern
     *
     * @param paramAddr - address pattern of the recieved parameter.
     *      Example: /tempo/.* represents tempo from all agent IDs
     * @param callback - the function to run if pattern is matched, should be of type:
     *      typedef std::function<void(const osc::ReceivedMessage& message)> callbackFunction;
     */
    virtual void addFunction(string paramAddr, CallbackFunction callback);

private:
    OscListener oscIn;

};


#endif //MATO_OSCMONITOR_H
