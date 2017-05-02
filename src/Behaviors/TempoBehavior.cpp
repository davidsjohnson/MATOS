//
// Created by David Johnson on 4/26/17.
//

#include "TempoBehavior.h"

extern int g_agentID;

TempoBehavior::TempoBehavior(Goal g) : goal(g)
{}

/*!
 * Initializes the behavior to handle OSC Input from neighbors
 *
 * @param beliefs
 * @param oscMonitor
 */
void TempoBehavior::init(Beliefs& beliefs, OscMonitor& oscMonitor){

    // Initialize with OSC Input Function from Neighbors
    callbackFunction tempoIn = [&](const osc::ReceivedMessage& message) {
        osc::ReceivedMessageArgumentIterator arg = message.ArgumentsBegin();
        // Add (or update) a belief for Tempo message from a neighbor
        try{
            float tempo = arg->AsFloat();  // only one argument in a tempo message (if there are more they will get ignored)
            (*beliefs)[message.AddressPattern()] = make_shared<Belief>("tempo", tempo);
        }
        catch(exception e){
            cout << "Error Adding Tempo Belief: " << e.what() << endl;
        }
    };
    oscMonitor.addFunction("/tempo/.*", tempoIn);
}


/*!
 * This function should add the required parameters for the corresponding
 * goal to blfParams
 *
 * @param beliefs - Pointer to the set of current Agent Beliefs
 * @param blfParams - map containing parameters for all goals
 */
void TempoBehavior::processBeliefs(const Beliefs beliefs, map<string, float>& blfParams) {

    // ##### Process Beliefs to extract goal parameters
    vector<float> tempos;
    for (auto beliefPair : *beliefs){
        Belief b = *beliefPair.second;

        if(b.paramName == "tempo"){
            if (beliefPair.first == "/tempo/"+to_string(g_agentID)){
                blfParams["myTempo"] = b.value;
            }
            else {
                tempos.push_back(b.value);
            }
        }
    }

    if (tempos.size() > 0){
        float avg = accumulate(tempos.begin(), tempos.end(), 0) / tempos.size();
        blfParams["worldTempo"] = avg;
    }
    else{
        blfParams["worldTempo"] = blfParams["myTempo"];
    }
}

/*!
 * Adds possible actions to the vector
 *
 * @param blfParams - map of generated parameter values
 * @param actions - a list of actions to perform and if they are required or not
 */
void TempoBehavior::generateAction(map<string, float>& blfParams, vector<pair<bool, Goal>>& actionGoals) {

    try{
        bool goalMet = goal.evaluate(blfParams);

        if (goalMet){
            pair<bool, Goal> p = make_pair(true, goal);
            actionGoals.push_back(p);
        }
        else{
            pair<bool, Goal> p = make_pair(false, goal);
            actionGoals.push_back(p);
        }

    }
    catch(MissingParameterException e){
        cout << "Error: Unable to Evaluate Goal: "<< e.what() << endl;
    }
}
