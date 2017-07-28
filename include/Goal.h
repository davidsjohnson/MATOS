//
// Created by David Johnson on 4/18/17.
//

#ifndef MATO_GOAL_H
#define MATO_GOAL_H

#include "common.h"

class Goal;

/**
 * The database of all agent goals
 */
typedef shared_ptr<vector<Goal, allocator<Goal>>> Goals;

/**
 * Callback function type for a goal's corresponding action
 */
typedef function<void(bool result, const Goal& g, map<string, float>& params)> ActionFunction;


class MissingParameterException : public runtime_error{
public:
    explicit MissingParameterException(const char* message) : runtime_error(message){};

};


/**
 * A goal represents an objective that an agent tries to meet and
 * an action that should be performed based on the evaluation of the
 * goal against the agents belief parameters.
 */
class Goal {

public:
    explicit Goal(ActionFunction callback);
    Goal(vector<string> infixExpression, ActionFunction callback);

    /**
     * sets the condition for the goal.
     *
     * @param infixExpression - the condition that should be evaluated for
     *      the specific goal.
     *      See Agent.cpp for an example of an infixExpression
     */
    void    setExpression(vector<string> infixExpression);

    /**
     * Evaluates the goals condition and returns the result
     *
     * @param params - belief parameters to use in the evaluation
     * @return  - result of the condition evalution
     */
    bool    evaluate(map<string, float>& params);

    /**
     * Call the action for the goal
     *
     * @param params - belief parameters that may be used in the action
     */
    void    action(map<string, float>& params);


    /**
     * @return - the goal's callback function
     */
    ActionFunction getAction() {return callback;}

private:

    vector<string>  outputQueue;
    ActionFunction  callback;

    friend ostream& operator<<(ostream& os, const Goal& goal);
};

#endif //MATO_GOAL_H
