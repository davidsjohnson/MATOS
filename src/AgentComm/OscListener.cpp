//
// Created by David Johnson on 4/18/17.
//

#include "OscListener.h"

OscListener::OscListener(int port) :
        osc::OscPacketListener(), m_port(port)
{
    //Default Callback function a osc Callback
    CallbackFunction func = [](const osc::ReceivedMessage& message){
        try{

            osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();

            cout << "OSC Received | Address: " << message.AddressPattern() << " | Args: ";
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

//    onReceive(".*", func); // Apply to All messages  TODO: Add debugging options...
}

OscListener::~OscListener() {
    if (t.joinable()) t.join();
}

void OscListener::start(){
    shared_ptr<UdpListeningReceiveSocket> socket = make_shared<UdpListeningReceiveSocket>(IpEndpointName(IpEndpointName::ANY_ADDRESS, m_port), this);
    t = thread(&OscListener::run, this, socket);
}

void OscListener::run(shared_ptr<UdpListeningReceiveSocket> socket) {
    socket->Run();
}

void OscListener::onReceive(string addressPattern, CallbackFunction callback) {
    messages[addressPattern] = callback;
}


void OscListener::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& endpoint) {

    (void) endpoint;  //suppress unused warnings

    for (auto callbackPair : messages){

        string address = callbackPair.first;
        regex r(address);

        if (regex_match(m.AddressPattern(), r)) {
            CallbackFunction callback = callbackPair.second;

            try {
                callback(m);
            }
            catch(const exception& e){
                cerr << "Error in Callback Function: " << callbackPair.first << " " << e.what() << endl;
            }
        }
    }
}
