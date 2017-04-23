//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_GOAL_H
#define MATO_GOAL_H

#include "common.h"

class Goal;
typedef function<void(bool result, const Goal& g, const map<string, int>& params)> ResultCallback;

class Goal {

public:
    Goal(ResultCallback callback);
    Goal(vector<string> infixExpression, ResultCallback callback);

    void    setExpression(vector<string> infixExpression);
    bool    evaluate(map<string, int> params);
    void    action(map<string, int> params);

    string  name;


private:

    vector<string>  outputQueue;
    ResultCallback  callback;

    friend ostream& operator<<(ostream& os, const Goal& goal);
};

#endif //MATO_GOAL_H
