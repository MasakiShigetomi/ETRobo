/******************************************************************************
 *  LineTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_
/* ライントレースのエッジ定義 */
#define R_EDGE 0 // 左エッジ
#define L_EDGE 1 // 右エッジ

/* カラーセンサの反射光の白黒判断基準値 */
#define WHITE 40 //白値判断基準閾値
#define BLACK 10 //黒値判断基準閾値

#define TASK_INTERVAL 4.0 // タスク周期

#include "LineMonitor.h"
#include "Walker.h"
#include "ev3api.h"
#include "Motor.h"       // <1>
#include "ColorSensor.h" // <2>
#include "Distance.h"
#include "ScenarioTracer.h"
#include "SimpleTimer.h"
#include "Starter.h"
#include "ReturnLine.h"

using namespace ev3api; // <4>
class Distance;
class ReturnLine;
class LineTracer
{
public:
    LineTracer(ScenarioTracer *scenarioTracer,
               const Starter *starter,
               Walker *walker,
               SimpleTimer *simpleTimer);

    void run();
    void linetrace_run();
    void LeftAndRightSetPWM(int pwm_l, int pwm_r);
    float pid_calc(float sensor_val, float target_val);
    float diff[2];
    float integral;

private:
    enum State
    {
        UNDEFINED,
        WAITING_FOR_START,
        LINE_TRACING,
        SCENARIO_TRACING
    };

    static const int MIN_TIME;
    static const int MAX_TIME;

    Walker *mWalker;
    ScenarioTracer *mScenarioTracer;
    SimpleTimer *mSimpleTimer;
    const Starter *mStarter;
    State mState;
    bool mIsInitialized;

    LineTracer *mLineTracer;
    Motor leftWheel;
    Motor rightWheel;
    ColorSensor colorSensor; // <3>

    int getRandomTime();
    void modeChangeAction();
    void execUndefined();
    void execWaitingForStart();
    void execLineTracing();
    void execScenarioTracing();
    float calc_prop_value();

    static const int8_t pwm = Motor::PWM_MAX;
};

#endif // EV3_APP_LINETRACER_H_
