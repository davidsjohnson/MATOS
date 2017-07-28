//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_OSCLISTENER_H
#define MATO_OSCLISTENER_H

#include "common.h"
#include <regex>

#include "oscpack/osc/OscPacketListener.h"
#include "oscpack/osc/OscReceivedElements.h"
#include "oscpack/ip/UdpSocket.h"


/**
 * Receives and processes OSC messages.  Used for communcation between agents and/or sensors
 */
class OscListener: public osc::OscPacketListener {

public:

    /**
     * Initializes the OSC listener and setups a default osc callback
     *
     * @param port
     */
    OscListener(int port);


    /**
     * Destroys the thread the listener is running in
     */
    ~OscListener();


    /**
     * Registers a callback function to the given address pattern
     *
     * @param addressPattern - string containing a valid regex pattern
     * @param callback - the function to run if pattern is matched, should be of type:
     *      typedef std::function<void(const osc::ReceivedMessage& message)> callbackFunction;
     */
    void onReceive(string addressPattern, CallbackFunction callback);


    /**
     * Starts the OSC listener as a new thread
     */
    void start();


    void stop();

private:

    int                                     m_port;
    map<string, CallbackFunction>           messages;

    thread t;
    shared_ptr<UdpListeningReceiveSocket>   m_socket;


    /**
     * Parses a new OSC message and calls the callback function that matches the address
     *
     * @param m - new OSC message
     * @param endpoint - IP address of sender
     */
    virtual void    ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& endpoint);


    /**
     * @param socket - socket to start on thread
     */
    void            run(shared_ptr<UdpListeningReceiveSocket> socket);


};


#endif //MATO_OSCMONITOR_H
