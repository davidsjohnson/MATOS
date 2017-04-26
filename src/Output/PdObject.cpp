//
// Created by David Johnson on 4/17/17.
//
#include "PdObject.h"
#include "Agent.h"

using namespace pd;

void PdObject::setAgent(Agent* agent) {
    m_agent = agent;
}

//--------------------------------------------------------------
void PdObject::print(const std::string& message) {
    cout << message << endl;
}

//--------------------------------------------------------------
void PdObject::receiveBang(const std::string& dest) {
    cout << "CPP: bang " << dest << endl;
}

void PdObject::receiveFloat(const std::string& dest, float num) {
    cout << "CPP: float " << dest << ": " << num << endl;

    // TODO:  Fix Circular reference issues...
    string exp = "\\d{4}-([a-zA-z]+?)-toCpp";
    regex rgx(exp);
    std::smatch match;

    if (std::regex_search(dest.begin(), dest.end(), match, rgx)) {

        m_agent->updateState(match[1], num);

        if (match[1] == "state") {
            int t = time(NULL) % 2592000;               // moding to reduce size of value to fit in float
            m_agent->updateState("stateChgTime", t);
        }
    }
    else {
        cerr << "Error: No Matching Input for: " << dest << endl;
    }
}

void PdObject::receiveSymbol(const std::string& dest, const std::string& symbol) {
    cout << "CPP: symbol " << dest << ": " << symbol << endl;
}

void PdObject::receiveList(const std::string& dest, const List& list) {
    cout << "CPP: list " << dest << ": ";

    // step through the list
    for(int i = 0; i < list.len(); ++i) {
        if(list.isFloat(i))
            cout << list.getFloat(i) << " ";
        else if(list.isSymbol(i))
            cout << list.getSymbol(i) << " ";
    }

    // you can also use the built in toString function or simply stream it out
    // cout << list.toString();
    // cout << list;

    // print an OSC-style type string
    cout << list.types() << endl;
}

void PdObject::receiveMessage(const std::string& dest, const std::string& msg, const List& list) {
    cout << "CPP: message " << dest << ": " << msg << " " << list.toString() << list.types() << endl;
}

//--------------------------------------------------------------
void PdObject::receiveNoteOn(const int channel, const int pitch, const int velocity) {
    cout << "CPP MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
}

void PdObject::receiveControlChange(const int channel, const int controller, const int value) {
    cout << "CPP MIDI: control change: " << channel << " " << controller << " " << value << endl;
}

void PdObject::receiveProgramChange(const int channel, const int value) {
    cout << "CPP MIDI: program change: " << channel << " " << value << endl;
}

void PdObject::receivePitchBend(const int channel, const int value) {
    cout << "CPP MIDI: pitch bend: " << channel << " " << value << endl;
}

void PdObject::receiveAftertouch(const int channel, const int value) {
    cout << "CPP MIDI: aftertouch: " << channel << " " << value << endl;
}

void PdObject::receivePolyAftertouch(const int channel, const int pitch, const int value) {
    cout << "CPP MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << endl;
}

void PdObject::receiveMidiByte(const int port, const int byte) {
    cout << "CPP MIDI: midi byte: " << port << " " << byte << endl;
}

