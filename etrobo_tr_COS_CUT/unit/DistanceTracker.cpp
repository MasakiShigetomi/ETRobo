#include <stdio.h>
#include "DistanceTracker.h"

DistanceTracker::DistanceTracker(ev3api::Motor& leftWheel,ev3api::Motor& rightWheel)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mTargetDistance(0) {
}

bool DistanceTracker::ReachedDistance() {
    //printf("%d\n",mTargetDistance);
    return (CountDistance() >= mTargetDistance) ? true : false;
}

int DistanceTracker::CountDistance() {
    int left_a = ev3_motor_get_counts(EV3_PORT_C );
    int right_a = ev3_motor_get_counts(EV3_PORT_B);
    return left_a + right_a;
}

void DistanceTracker::SetDistance(int distance) {
    mTargetDistance = distance;
}

