#ifndef EV3_UNIT_DISTANCETRACKER_H_
#define EV3_UNIT_DISTANCETRACKER_H_

#include "Motor.h"

class DistanceTracker {
public:
    DistanceTracker(ev3api::Motor& leftWheel,ev3api::Motor& rightWheel);
    bool ReachedDistance();
    int CountDistance();
    void SetDistance(int distance);
    

private:
    ev3api::Motor& mLeftWheel;
    ev3api::Motor& mRightWheel;
    int mTargetDistance;
};

#endif