#include "Arduino.h"
#include "Arm.h"

arm::arm(int n, int s)
{ 
    note = n;
    striker = s;
    strikerState = LOW;
}  
