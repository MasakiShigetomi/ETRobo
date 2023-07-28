/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/
#include "LineTracer.h"
#include "DistanceTracker.h"
#include <stdio.h>
#include<math.h>

/**
 * コンストラクタ
 */
LineTracer::LineTracer(const LineMonitor* lineMonitor,
                       Walker* walker,
                       DistanceTracker* distanceTracker)
    : mLineMonitor(lineMonitor),
      mWalker(walker),
      mDistanceTracker(distanceTracker),
      leftWheel(PORT_C),
      rightWheel(PORT_B),
      colorSensor(PORT_2),
      mIsInitialized(false) {
}

void LineTracer::SetPWM(int pwm_l, int pwm_r) {
    rightWheel.setPWM(pwm_r);
    leftWheel.setPWM(pwm_l);
    //走行距離表示
    //printf("%d\n", mDistanceTracker->CountDistance());
}


/**
 * ライントレースする
 */
void LineTracer::run() {
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float turn = calc_prop_value(colorSensor.getBrightness()); // <1>
    int pwm_l = pwm - turn;      // <2>
    int pwm_r = pwm + turn;      // <2>
    SetPWM(pwm_r * 0.7, pwm_l * 0.7);
}

/**
 * ライントレースの計算を行う
 */
float LineTracer::calc_prop_value(float color_val) {

    const float DELTA_T = 0.004;
    const float Kp = 0.9;        
    const float Ki = 0.016;
    const float Kd = 0.005;
    const int target = 10;        

    static float integral = 0;
    static float prev_error = 0;

    int diff[2] = {0, 0};
    diff[0] = diff[1];
    diff[1] = color_val - target;
    integral += (diff[1] + diff[0]) / 2.0 * DELTA_T;

    float p = Kp * diff[1];
    float i = Ki * integral;
    float d = Kd * (diff[1] - diff[0]) / DELTA_T;
    //P値表示
    //printf("p = %.2f\n", p);

    return p + i + d; //モーターの制御値
}



