//
// Created by David Johnson on 4/17/17.
//

#ifndef MATO_PDOBJECT_H
#define MATO_PDOBJECT_H

#include "common.h"

#include "PdBase.hpp"

class Agent;

// custom receiver class
class PdObject : public pd::PdReceiver, public pd::PdMidiReceiver {

public:

    void setAgent(Agent* agent);

    // pd message receiver callbacks
    void print(const std::string& message);

    void receiveBang(const std::string& dest);
    void receiveFloat(const std::string& dest, float num);
    void receiveSymbol(const std::string& dest, const std::string& symbol);
    void receiveList(const std::string& dest, const pd::List& list);
    void receiveMessage(const std::string& dest, const std::string& msg, const pd::List& list);

    // pd midi receiver callbacks
    void receiveNoteOn(const int channel, const int pitch, const int velocity);
    void receiveControlChange(const int channel, const int controller, const int value);
    void receiveProgramChange(const int channel, const int value);
    void receivePitchBend(const int channel, const int value);
    void receiveAftertouch(const int channel, const int value);
    void receivePolyAftertouch(const int channel, const int pitch, const int value);

    void receiveMidiByte(const int port, const int byte);

private:
    Agent* m_agent;
};


#endif //MATO_PDOBJECT_H
