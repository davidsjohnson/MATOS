//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_PDPATCH_H
#define MATO_PDPATCH_H

#include <string>
#include <iostream>
#include <initializer_list>

#include "RtAudio.h"

#include "PdBase.hpp"
#include "PdObject.h"

using namespace std;
using namespace pd;

//typedef function<int(void*, void*, unsigned int, double, RtAudioStreamStatus, void*)> AudioCallback;

class PdPatch {

public:

    PdPatch(const string& filename);
    ~PdPatch();

    void        sendStart();
    void        sendNextState();
    void        sendTempo(float tempo);
    void        sendParameters(const string& receiver, initializer_list<float> args);

private:

    PdBase      pd;
    PdObject    pdO;
    Patch       patch;
    shared_ptr<RtAudio>     audio;

    unsigned int    srate;
    unsigned int    n_bufferFrames;

    void        init(const string& filename);

    friend int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* userData);
};


#endif //MATO_PDPATCH_H
