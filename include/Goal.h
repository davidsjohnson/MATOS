//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_GOAL_H
#define MATO_GOAL_H

#include "common.h"

//class Goal;
//
//typedef function<void(bool result, const Goal& g)> ResultCallback;

class Goal {

public:
    Goal(function<void(bool result, const Goal& g)> callback);
    Goal(vector<string> infixExpression, function<void(bool result, const Goal& g)> callback);

    string name;

    void setExpression(vector<string> infixExpression);
    bool evaluate(map<string, int> params);
    void runFunction(map<string, int> params);


private:

    vector<string>  outputQueue;
    function<void(bool result, const Goal& g)>  callback;

    friend ostream& operator<<(ostream& os, const Goal& goal);
};

#endif //MATO_GOAL_H
