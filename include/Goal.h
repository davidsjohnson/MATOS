//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_GOAL_H
#define MATO_GOAL_H

#include "common.h"

class Goal;
typedef shared_ptr<vector<Goal, allocator<Goal>>> Goals;
typedef function<void(bool result, const Goal& g, map<string, float>& params)> ResultCallback;

class Goal {

public:
    Goal(ResultCallback callback);
    Goal(vector<string> infixExpression, ResultCallback callback);

    void    setExpression(vector<string> infixExpression);
    bool    evaluate(map<string, float> params);
    void    action(map<string, float>& params);

    string  name;


private:

    vector<string>  outputQueue;
    ResultCallback  callback;

    friend ostream& operator<<(ostream& os, const Goal& goal);
};

#endif //MATO_GOAL_H
