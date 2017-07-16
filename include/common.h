//
// Created by David Johnson on 4/19/17.
//

#ifndef MATO_COMMON_H
#define MATO_COMMON_H

#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <stack>
#include <queue>
#include <thread>
#include <time.h>
#include <sstream>
#include <fstream>
#include <numeric>
#include <unistd.h>

#include "oscpack/osc/OscReceivedElements.h"

using namespace std;

typedef std::function<void(const osc::ReceivedMessage& message)> CallbackFunction;

#endif //MATO_COMMON_H
