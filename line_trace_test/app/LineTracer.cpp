/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracer.h"
#include "Distance.h"
#include <stdlib.h>
#include "Clock.h"
#include "ReturnLine.h"

#define DELTA_T 0.004 // 処理周期と同刻にする
#define KP 4.6        // P（比例制御）係数
#define KI 2.4        // I（積分制御）係数
#define KD 0.00015    // D（微分制御）係数
// 定数宣言
const int LineTracer::MIN_TIME = 5000 * 1000;  // 切り替え時間の最小値
const int LineTracer::MAX_TIME = 15000 * 1000; // 切り替え時間の最大値

/**
 * コンストラクタ
 * @param scenarioTracer  シナリオトレーサ
 * @param starter         スタータ
 * @param walker 走行
 * @param simpleTimer     タイマ
 */
LineTracer::LineTracer(ScenarioTracer *scenarioTracer,
                       const Starter *starter,
                       Walker *walker,
                       SimpleTimer *simpleTimer)
    : mWalker(walker),
      mScenarioTracer(scenarioTracer),
      mSimpleTimer(simpleTimer),
      mStarter(starter),
      mState(UNDEFINED),
      mIsInitialized(false),
      leftWheel(PORT_C),
      rightWheel(PORT_B),
      colorSensor(PORT_2)
{
}
Distance distance;
ReturnLine returnline;

int pwm_l = 0;
int pwm_r = 0;
int case_number = 0;
float turn = 0;
float diff[2];
float integral;

/**
 * ライントレースする
 */
void LineTracer::linetrace_run()
{
    if (mIsInitialized == false)
    {
        mWalker->init();
        mIsInitialized = true;
    }

    float dis = distance.Distance_getDistance();
    float disL = distance.Distance_getTotalDistanceL();
    float disR = distance.Distance_getTotalDistanceR();

    turn = pid_calc(colorSensor.getBrightness(), 12.5);
    pwm_l = pwm - turn;
    pwm_r = pwm + turn;
    LeftAndRightSetPWM(pwm_r * 0.7, pwm_l * 0.7);
}

// pid制御のための計算
float LineTracer::pid_calc(float sensor_val, float target_val)
{

    float p, i, d;

    diff[0] = diff[1];
    diff[1] = sensor_val - target_val;
    integral += (diff[1] + diff[0]) / 2.0 * DELTA_T;

    p = KP * diff[1];
    i = KI * integral;
    d = KD * (diff[1] - diff[0]) / DELTA_T;
    return p + i + d;
}

// 左右モータに走行指示を与える
void LineTracer::LeftAndRightSetPWM(int pwm_l, int pwm_r)
{
    // Lコースの場合
    rightWheel.setPWM(pwm_r);
    leftWheel.setPWM(pwm_l);
}

/*
 * p制御のための計算
 */
float LineTracer::calc_prop_value()
{
    const float Kp = 0.83;
    const int target = 12.5;
    // const int target = 15;
    // const int target = 5;
    const int bias = 0;

    int diff = colorSensor.getBrightness() - target;
    return (Kp * diff + bias);
}

/**
 * 走行する
 */
void LineTracer::run()
{
    switch (mState)
    {
    case UNDEFINED:
        execUndefined();
        break;
    case WAITING_FOR_START:
        execWaitingForStart();
        break;
    case LINE_TRACING:
        execLineTracing();
        break;
    case SCENARIO_TRACING:
        execScenarioTracing();
        break;
    default:
        break;
    }
}

/**
 * 乱数を取得する
 * @retrun 乱数
 */
int LineTracer::getRandomTime()
{
    return MIN_TIME +
           static_cast<int>(static_cast<double>(rand()) *
                            (MAX_TIME - MIN_TIME + 1.0) / (1.0 + RAND_MAX));
}

/**
 * シーン変更処理
 */
void LineTracer::modeChangeAction()
{
    // 　本当は普通にシナリオを次に持っていきたい。
    mSimpleTimer->setTime(getRandomTime());
    mSimpleTimer->start();
}

/**
 * 未定義状態の処理
 */
void LineTracer::execUndefined()
{
    mState = WAITING_FOR_START;
}

/**
 * 開始待ち状態の処理
 */
void LineTracer::execWaitingForStart()
{
    if (mStarter->isPushed())
    {
        mState = LINE_TRACING;

        modeChangeAction();
    }
}

/**
 * ライントレース状態の処理
 */
void LineTracer::execLineTracing()
{
    linetrace_run();

    // if (mSimpleTimer->isTimedOut())
    // {
    //     mSimpleTimer->stop();

    //     mState = SCENARIO_TRACING;

    //     modeChangeAction();
    // }
}

/**
 * シナリオトレース状態の処理
 */
void LineTracer::execScenarioTracing()
{
    mScenarioTracer->run();

    if (mSimpleTimer->isTimedOut())
    {
        mSimpleTimer->stop();

        mState = LINE_TRACING;

        modeChangeAction();
    }
}