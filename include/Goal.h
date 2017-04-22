//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_GOAL_H
#define MATO_GOAL_H

#include "common.h"

class Goal {

public:
    Goal();
    Goal(vector<string>);

    string name;

    void setExpression(vector<string> infixExpression);
    bool evaluate(map<string, int> parameterMap);


private:
    vector<string> outputQueue;

    friend ostream& operator<<(ostream& os, const Goal& goal);
};


#endif //MATO_GOAL_H
