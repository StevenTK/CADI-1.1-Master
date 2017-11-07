#ifndef Arm_h
#define Arm_h

#include "Arduino.h"

class arm{
public:  
    arm(int n, int s);
    int note;
    int striker;
    int strikerState;
};

#endif
