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

class OscListener: public osc::OscPacketListener {

public:

    OscListener(int port);
    void onReceive(string addressPattern, callbackFunction callback);
    void start();

private:

    int                           m_port;
    map<string, callbackFunction> messages;

    virtual void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& endpoint);

};


#endif //MATO_OSCMONITOR_H
