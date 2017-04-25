//
// Created by David Johnson on 4/18/17.
//

#include "OscListener.h"

OscListener::OscListener(int port) :
        osc::OscPacketListener(), m_port(port)
{
    //Default Callback function a osc Callback
    callbackFunction func = [](const osc::ReceivedMessage& message){
        try{

            osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();

            cout << "OSC Received | Args: ";
            for (; arg!= message.ArgumentsEnd(); arg++)
            {
                if(arg->IsFloat()){
                    cout << arg->AsFloat() << " ";
                }
                else if (arg->IsString()){
                    cout << arg->AsString() << " ";
                }
                else if (arg->IsInt32()){
                    cout << arg->AsInt32() << " ";
                }
                else if (arg->IsInt64()){
                    cout << arg->AsInt64() << " ";
                }
                else{
                    cout << "Type Not Supported for default message"  << " ";
                }
            }
            cout << endl;

        }
        catch(osc::Exception& e){
            cout << "Error: Error while parsing OSC Message " << message.AddressPattern() << ": " << e.what() << endl;
        }
    };

    onReceive(".*", func); // Apply to All messages
}

OscListener::~OscListener() {
    if (t.joinable()) t.join();
}

void OscListener::start(){
    t = thread(&OscListener::run, this);
}

void OscListener::run() {
    UdpListeningReceiveSocket socket( IpEndpointName(IpEndpointName::ANY_ADDRESS, m_port), this );
    socket.Run();
}

void OscListener::onReceive(string addressPattern, callbackFunction callback) {
    messages[addressPattern] = callback;
}


void OscListener::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& endpoint) {

    (void) endpoint;  //suppress unused warnings

    for (auto callbackPair : messages){

        string address = callbackPair.first;
        regex r(address);

        if (regex_match(m.AddressPattern(), r)) {
            callbackFunction callback = callbackPair.second;

            try {
                callback(m);
            }
            catch(const exception& e){
                cerr << "Error in Callback Function: " << callbackPair.first << " " << e.what() << endl;
            }
        }
    }
}
