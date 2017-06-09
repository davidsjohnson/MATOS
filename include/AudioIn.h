//
// Created by David Johnson on 5/29/17.
//

#ifndef MATO_AUDIOMONITOR_H
#define MATO_AUDIOMONITOR_H

#include "RtAudio.h"

#include "common.h"


class AudioIn {

public:
    AudioIn(unsigned int srate=44100, unsigned int n_bufferframes=128);

private:

    unsigned int srate;
    unsigned int n_bufferFrames;
    RtAudio audioIn;

    friend int audioInCallback(void *outputBuffer, void *inputBuffer, unsigned int n_bufferFrames, double streamTime, RtAudioStreamStatus status, void* userData);

};


#endif //MATO_AUDIOMONITOR_H
