//
// Created by David Johnson on 5/29/17.
//

#include "AudioIn.h"


int audioInCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime,
                    RtAudioStreamStatus status, void *userData) {

    // Do something with Audio Input Here
    // Record for Granular Synthesis
    return 0;
}


void audioInErrorCallback(RtAudioError::Type type, const string& errorText){
    cerr << "PdAudio Error: " << errorText << endl;
};


AudioIn::AudioIn(unsigned int srate, unsigned int n_bufferFrames) :
srate(srate), n_bufferFrames(n_bufferFrames),
audioIn()
{


    // Initialize Rt Audio Input Stream
    if(audioIn.getDeviceCount()==0){
        cout << "Error: There are no available sound devices." << endl;
        exit(1);
    }

    RtAudio::StreamParameters inputParameters;
    inputParameters.deviceId = audioIn.getDefaultInputDevice();
    inputParameters.nChannels = 2;

    RtAudio::StreamOptions options;
    options.streamName = "Tangible Agent";
    options.flags = RTAUDIO_SCHEDULE_REALTIME;
    if ( audioIn.getCurrentApi() != RtAudio::MACOSX_CORE){
        options.flags |= RTAUDIO_MINIMIZE_LATENCY;  // CoreAudio doesn't like this
    }

    try{
        audioIn.openStream(NULL, &inputParameters, RTAUDIO_FLOAT32, srate, &n_bufferFrames, &audioInCallback, this, &options, &audioInErrorCallback);
        audioIn.startStream();
    }
    catch ( RtAudioError& e){
        cerr << e.getMessage() << endl;
        exit(1);
    }

}