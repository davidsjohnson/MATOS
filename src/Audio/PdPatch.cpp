//
// Created by David Johnson on 4/18/17.
//

#include "PdPatch.h"

int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData) {

            int ticks = nBufferFrames / 64;

            PdPatch* pd = (PdPatch*)userData;
            pd->pd.processFloat(ticks, (float*)inputBuffer, (float*)outputBuffer);

    return 0;
};

void errorCallback(RtAudioError::Type type, const string& errorText){
    cerr << "Audio Error: " << errorText << endl;
};


PdPatch::PdPatch(const string& filename) :
    audio(make_shared<RtAudio>()),
    srate(44100),
    n_bufferFrames(128)

{
    init(filename);
}

PdPatch::~PdPatch() {
    audio->closeStream();
}


void PdPatch::init(const string &filename) {

    if(!pd.init(0, 2, srate)) {
        cerr << "Could not initialize PD" << endl;
        exit(1);
    }

    pd.computeAudio(true);
    pd.addToSearchPath("../in_c/patch_editor_abs");

    pd.setReceiver(&pdO);

    patch = pd.openPatch(filename, "..");
    if (!patch.isValid())
    {
        cerr << "Error loading patch" << endl;
        exit(1);
    }

    if(audio->getDeviceCount()==0){
        cout << "Error: There are no available sound devices." << endl;
        exit(1);
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = audio->getDefaultOutputDevice();
    parameters.nChannels = 2;

    RtAudio::StreamOptions options;
    options.streamName = "Tangible Agent";
    options.flags = RTAUDIO_SCHEDULE_REALTIME;
    if ( audio->getCurrentApi() != RtAudio::MACOSX_CORE){
        options.flags |= RTAUDIO_MINIMIZE_LATENCY;  // CoreAudio doesn't like this
    }

    try{
        audio->openStream(&parameters, NULL, RTAUDIO_FLOAT32, srate, &n_bufferFrames, &audioCallback, this, &options, &errorCallback);
        audio->startStream();
    }
    catch ( RtAudioError& e){
        cerr << e.getMessage() << endl;
        exit(1);
    }

    sendNextState();
    sendParameters("1005-maestro4-ch1-r", {100.0f});
    sendParameters("1005-maestro4-master-r", {100.0f});
}


void PdPatch::sendStart(){
    pd.sendBang(patch.dollarZeroStr() + "-start");
}


void PdPatch::sendNextState(){
    pd.sendBang(patch.dollarZeroStr() + "-next_state");
}


void PdPatch::sendTempo(float tempo){
    pd.sendFloat(patch.dollarZeroStr() + "-set_tempo", tempo);
}


void PdPatch::sendParameters(const string &receiver, initializer_list<float> args) {
    pd.startMessage();
    for (float f : args){
        pd.addFloat(f);
    }
    pd.finishList(receiver);
}



