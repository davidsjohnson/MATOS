//
// Created by David Johnson on 4/18/17.
//

#include "Goal.h"

bool is_operator(string token){
    return token == "<" || token == ">" || token == "==" || token == ">=" || token == "<=" || token == "+" || token == "*" || token == "/" || token == "-";
}

int precedence(string token){

    if (token == "*" || token == "/") return 3;
    if (token == "+" || token == "-") return 2;
    if (token == "<" || token == ">" || token == "==" || token == ">=" || token == "<=" ) return 1;
    else throw exception();
}

int evaluateArith(int operand1, int operand2, string op){

    if (op == "+") return operand2 + operand1;
    if (op == "-") return operand2 - operand1;
    if (op == "*") return operand2 * operand1;
    if (op == "/") return operand2 / operand1;

    return 0;
}

bool evaluateCondition(int operand1, int operand2, string op){

    if (op == "<") return operand2 < operand1;
    if (op == "<=") return operand2 <= operand1;
    if (op == "==") return operand2 == operand1;
    if (op == ">=") return operand2 >= operand1;
    if (op == ">") return operand2 > operand1;

    return true;
}

Goal::Goal() :
        outputQueue()
{}

Goal::Goal(vector<string> infixExpression) :
        outputQueue()
{
    setExpression(infixExpression);
}


void Goal::setExpression(vector<string> infixExpression) {

    stack<string> operatorStack;

    for (string token : infixExpression){
        // Check of operator is an operator
        if (is_operator(token)) {

                while (!operatorStack.empty() && precedence(token) <= precedence(operatorStack.top()) ){
                    outputQueue.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.push(token);
        }
        // if not an operator than must be a variable or number
        else{
            outputQueue.push_back(token);
        }
    }

    while(!operatorStack.empty()){
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }
}


bool Goal::evaluate(map<string, int> parameterMap){

    if (outputQueue.empty())
        throw exception();

    stack<string> evalStack;
    queue<string, deque<string>> evalQueue(deque<string>(outputQueue.begin(), outputQueue.end()));

    while (!evalQueue.empty()){

        string test = evalQueue.front();

        if (!is_operator(evalQueue.front())){
            evalStack.push(evalQueue.front());
            evalQueue.pop();
        }
        else{
            string operand1 = evalStack.top();
            evalStack.pop();
            string operand2 = evalStack.top();
            evalStack.pop();

            string op = evalQueue.front();
            evalQueue.pop();

            int o1;
            int o2;

            try {
                o1 = parameterMap.at(operand1);
            }
            catch(exception e){
                o1 = stoi(operand1);
            }

            try {
                o2 = parameterMap.at(operand2);
            }
            catch(exception e){
                o2 = stoi(operand2);
            }


            if (op == "+" || op =="-" || op == "*" || op == "/"){
                int value = evaluateArith(o1, o2, op);
                evalStack.push(to_string(value));
            }
            else{
                return evaluateCondition(o1, o2, op);
            }
        }

    }
    return false;
}


ostream&operator<<(ostream& os, const Goal& goal){

    for (string token : goal.outputQueue){
        os << token << " " ;
    }
    return os;
}
