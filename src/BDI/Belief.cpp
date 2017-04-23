//
// Created by David Johnson on 4/18/17.
//

#include "Belief.h"



ostream& operator<<(ostream& os, const Belief& belief){
    os << "(" << belief.paramName << ", " << belief.value << ")";
    return os;
}

