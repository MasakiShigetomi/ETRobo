/******************************************************************************
 *  LineTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_



#include "LineMonitor.h"
#include "DistanceTracker.h"
#include "Motor.h"
#include "ColorSensor.h"

using namespace ev3api;

class LineTracer {
public:
    LineTracer(const LineMonitor* lineMonitor,
               DistanceTracker* distanceTracker);

    int run();
    void SetPWM(int pwm_l, int pwm_r);

private:
    const LineMonitor* mLineMonitor;
    DistanceTracker* mDistanceTracker;
    bool mIsInitialized;
    int calcDirection(bool isOnLine);
    Motor leftWheel;
    Motor rightWheel;
    ColorSensor colorSensor;
    
    static const int8_t pwm = Motor::PWM_MAX;

    float calc_prop_value(float color_val);  
};

#endif  // EV3_APP_LINETRACER_H_
