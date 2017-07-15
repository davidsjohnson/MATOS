//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_PDPATCH_H
#define MATO_PDPATCH_H

#include "common.h"

#include "RtAudio.h"

#include "PdBase.hpp"
#include "PdObject.h"

using namespace pd;

class Agent;

/**
 * runs the pd patch specified in the constructor
 */
class PdPatch {

public:

    PdPatch(const string& filename);
    ~PdPatch();

    void        sendStart(float tempo);
    void        sendState(int state_num);
    void        sendTempo(float tempo);
    void        sendParameters(const string& receiver, initializer_list<float> args);
    void        sendBang(const string& dest);
    string      dollarZeroStr();

    void        init(Agent* agent);

private:

    PdBase                  pd;
    PdObject                pdO;
    Patch                   patch;
    shared_ptr<RtAudio>     audio;
    string                  pdFile;

    unsigned int    srate;
    unsigned int    n_bufferFrames;

    friend int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* userData);
};


#endif //MATO_PDPATCH_H
