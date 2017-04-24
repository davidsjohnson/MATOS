//
// Created by David Johnson on 4/22/17.
//

#ifndef MATO_OSCSENDER_H
#define MATO_OSCSENDER_H

#include "oscpack/osc/OscOutboundPacketStream.h"
#include "oscpack/ip/UdpSocket.h"

#define OUTPUT_BUFFER_SIZE 1024


class OscSender {

public:

OscSender(int port=7000, string  ipAddress="127.0.0.1") :
        m_transmitSocket( IpEndpointName(ipAddress.c_str(), port) )
{
   cout << "OSC Sending to:" << ipAddress << " port: " << port << endl;
}

    template<typename T, typename... Ts>
    void sendOsc(string address, T arg, Ts... args)
    {
        char buffer[OUTPUT_BUFFER_SIZE];
        osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

        p << osc::BeginMessage( address.c_str() );
        buildPacket(p, arg, args...);
        p << osc::EndMessage;

        m_transmitSocket.Send( p.Data(), p.Size() );
    }

private:
    UdpTransmitSocket m_transmitSocket;

    void buildPacket(osc::OutboundPacketStream &p){}

    template<typename T, typename... Ts>
    void buildPacket(osc::OutboundPacketStream& p, T arg, Ts... args)
    {
        p << arg;
        buildPacket(p, args...);
    }

};



#endif //MATO_OSCSENDER_H
