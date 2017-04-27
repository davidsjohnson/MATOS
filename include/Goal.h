//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_GOAL_H
#define MATO_GOAL_H

#include "common.h"

class Goal;
typedef shared_ptr<vector<Goal, allocator<Goal>>> Goals;
typedef function<void(bool result, const Goal& g, map<string, float>& params)> ActionFunction;

class MissingParameterException : public runtime_error{
public:
    MissingParameterException(const char* message) : runtime_error(message){};

};

class Goal {

public:
    Goal(ActionFunction callback);
    Goal(vector<string> infixExpression, ActionFunction callback);

    void    setExpression(vector<string> infixExpression);
    bool    evaluate(map<string, float>& params);
    void    action(map<string, float>& params);
    ActionFunction getAction() {return callback;}

    string  name;


private:

    vector<string>  outputQueue;
    ActionFunction  callback;

    friend ostream& operator<<(ostream& os, const Goal& goal);
};

#endif //MATO_GOAL_H
