/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/
#include "LineTracer.h"
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

/**
 * ライントレースする
 */
void LineTracer::run() {
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }
    float turn = calc_prop_value(colorSensor.getBrightness()); // <1>
    int intTurn = static_cast<int>(turn);
    mWalker->run(intTurn); //PID制御値を引数に、walkerのrunメソッドを呼び出す
}

/**
 * PID制御の計算を行う
 */
float LineTracer::calc_prop_value(float color_val) {
    const float DELTA_T = 0.004;
    const float Kp = 0.9;        // <1>
    const float Ki = 0.016;
    const float Kd = 0.005;
    const int target = 10;        // <2>

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

    return p + i + d;
}



