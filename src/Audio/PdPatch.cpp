//
// Created by David Johnson on 4/18/17.
//

#include "PdPatch.h"

int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData) {

//    for (int i=0; i<nBufferFrames; ++i){
//        cout << ((float*)inputBuffer)[i] << endl;
//    }

    int ticks = nBufferFrames / 64;

    PdPatch* pd = (PdPatch*)userData;
    pd->pd.processFloat(ticks, (float*)inputBuffer, (float*)outputBuffer);

//    memcpy(outputBuffer, inputBuffer, sizeof(float)*nBufferFrames);

    return 0;
};

void errorCallback(RtAudioError::Type type, const string& errorText){
    cerr << "PdAudio Error: " << errorText << endl;
};


PdPatch::PdPatch(const string& filename) :
    audio(make_shared<RtAudio>()),
    srate(44100),
    n_bufferFrames(128),
    pdFile(filename)
{
}

PdPatch::~PdPatch() {
    audio->closeStream();
}


void PdPatch::init(Agent* agent) {

    if(!pd.init(2, 2, srate)) {
        cerr << "Could not initialize PD" << endl;
        exit(1);
    }

    pd.computeAudio(true);
    pd.addToSearchPath("../pd/patch_editor_abs");

    pd.setReceiver(&pdO);
    pdO.setAgent(agent);        // TODO: Try not to have to set agent here

    patch = pd.openPatch(pdFile, "..");
    if (!patch.isValid())
    {
        cerr << "Error loading patch" << endl;
        exit(1);
    }

    // TODO: Turn into public api method
    pd.subscribe(patch.dollarZeroStr()+ "-tempo-toCpp");
    pd.subscribe(patch.dollarZeroStr()+ "-state-toCpp");
    pd.subscribe(patch.dollarZeroStr()+ "-volume-toCpp");
    pd.subscribe(patch.dollarZeroStr()+ "-start-toCpp");


    if(audio->getDeviceCount()==0){
        cout << "Error: There are no available sound devices." << endl;
        exit(1);
    }

    RtAudio::StreamParameters outputParameters;
    outputParameters.deviceId = audio->getDefaultOutputDevice();
    outputParameters.nChannels = 2;

    RtAudio::StreamParameters inputParameters;
    inputParameters.deviceId = audio->getDefaultInputDevice();
    inputParameters.nChannels = 2;

    RtAudio::StreamOptions options;
    options.streamName = "Tangible Agent";
    options.flags = RTAUDIO_SCHEDULE_REALTIME;
    if ( audio->getCurrentApi() != RtAudio::MACOSX_CORE){
        options.flags |= RTAUDIO_MINIMIZE_LATENCY;  // CoreAudio doesn't like this
    }

    try{
        audio->openStream(&outputParameters, &inputParameters, RTAUDIO_FLOAT32, srate, &n_bufferFrames, &audioCallback, this, &options, &errorCallback);
        audio->startStream();
    }
    catch ( RtAudioError& e){
        cerr << e.getMessage() << endl;
        exit(1);
    }

    sendState(0);
    sendParameters("-volume-fromCpp", {40.0f});
}

void PdPatch::closePatch(){
    pd.closePatch(patch);
}


void PdPatch::sendStart(){
    pd.sendBang(patch.dollarZeroStr() + "-start-fromCpp");
}


void PdPatch::sendStart(float tempo){
    pd.sendFloat(patch.dollarZeroStr() + "-start-fromCpp", tempo);
}


void PdPatch::sendState(int state_num) {
    pd.sendFloat(patch.dollarZeroStr() + "-state-fromCpp", state_num);
}


void PdPatch::sendTempo(float tempo){
    pd.sendFloat(patch.dollarZeroStr() + "-tempo-fromCpp", tempo);
}

void PdPatch::sendBang(const string& dest){
    pd.sendBang(patch.dollarZeroStr() + dest);
}


void PdPatch::sendParameters(const string &receiver, initializer_list<float> args) {
    stringstream ss;
    ss << patch.dollarZeroStr() << "-" << receiver;
    pd.startMessage();
    for (float f : args){
        pd.addFloat(f);
    }
    pd.finishList(ss.str());
}

string PdPatch::dollarZeroStr() {
    return patch.dollarZeroStr();
}



