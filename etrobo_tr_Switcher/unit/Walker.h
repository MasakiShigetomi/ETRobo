/******************************************************************************
 *  Walker.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class Walker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_WALKER_H_
#define EV3_UNIT_WALKER_H_

#include "Motor.h"
#include "GyroSensor.h"
#include "SimpleTimer.h"
#include "LineTracer.h"
#include "ev3api.h"

class Walker {
public:
    static const int LOW;
    static const int MID;
    static const int HIGH;
    
    static const int STRAIGHT;
    static const int RIGHTS;
    static const int LEFT;
    static const int STOP;
    static const int BACK;
    static const int BACKRIGHT;
    static const int BACKLEFT;
    static const int CLOCKWISE;
    static const int ACLOCKWISE;
    static const int LINETRACE;
    static const int STRAIGHTS;
    static const int RIGHT;

    Walker(ev3api::Motor& leftWheel,
                    ev3api::Motor& rightWheel,
                    SimpleTimer* WTimer,
                    LineTracer* lineTracer);

    void init();
    void run(int ContVal = 10000);
    void setCommand(int turn);
    void setSpeed(int forward);
    void setup();
    int calcScurve(float AT, int Pmax, int Pmin, int repeat, bool accel);

private:

    SimpleTimer* mSimpleTimer;
    LineTracer* mLineTracer;
    ev3api::Motor& mLeftWheel;
    ev3api::Motor& mRightWheel;
    int mForward;
    int mTurn;
    bool first_iteration;
};


#endif  // EV3_UNIT_WALKER_H_
