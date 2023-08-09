/******************************************************************************
 *  Walker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Class Walker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/
#include <stdlib.h>
#include "Walker.h"
#include "ev3api.h"
#include <time.h>
#include <cmath>
#include "Clock.h"

// 定数宣言
const int Walker::LOW    = 30;    // 低速
const int Walker::NORMAL = 50;    // 通常
const int Walker::HIGH   = 100;    // 高速

const int Walker::STRAIGHT = 0;
const int Walker::RIGHT  = 1;     // 左方向
const int Walker::LEFT   = 2;     // 右方向
const int Walker::STOP   = 3;
const int Walker::BACK   = 4;
const int Walker::BACKRIGHT  = 5;
const int Walker::BACKLEFT   = 6;
const int Walker::SAFESTOP  = 7;
const int Walker::BSAFESTOP = 8;
const int Walker::STARTBACK = 9;
const int Walker::FWSTRAIGHT = 10;
const int Walker::ROLLLEFT = 11;
const int Walker::LINETRACE  = 12;

float pForward;
static double prev_time_sum = 0; //経過時間の合計

bool first_iteration = true;
static int counter = 0;
/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
Walker::Walker(ev3api::Motor& leftWheel,
                                 ev3api::Motor& rightWheel,
                                 SimpleTimer* WTimer,
                                 LineTracer* lineTracer)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mForward(LOW),
      mTurn(RIGHT),
      mSimpleTimer(WTimer),
      mLineTracer(lineTracer) {

        //無くても動くことが判明
    //ev3api::Clock* clock = new ev3api::Clock();

    //srand(clock->now());  // 乱数をリセットする

    //delete clock;
}

/**
 * 走行する
 */
void Walker::run(int ContVal) {
    
    int rightPWM = 0; // 左右モータに回転を指示する
    int leftPWM = 0;
    
    if(mTurn == STRAIGHT) {
        int pForward = calcScurve(1.2, 100, 5, 1, true);
        
        rightPWM = -pForward; //モーターにパワーを設定する
        leftPWM =  -pForward;
        
    } else if(mTurn == RIGHT) {
        int pForward = calcScurve(2.18, 100, 52, 2, false);
        
        rightPWM = -pForward; 
        leftPWM =  -mForward;
    } else if(mTurn == LEFT) {
        int pForward = calcScurve(2.18, 100, 52, 2, false);
        
        rightPWM = -mForward; 
        leftPWM =  -pForward;
    } else if(mTurn == STOP) {
        rightPWM = 0;
        leftPWM = 0;
    } else if(mTurn == BACK) {
        rightPWM = -mForward;
        leftPWM = -mForward;
    } else if(mTurn == BACKRIGHT) {
        int pForward = calcScurve(2, 100, 38, 2, false);
        
        rightPWM = pForward; 
        leftPWM =  mForward;
    } else if(mTurn == BACKLEFT) {
        rightPWM = 0;
        leftPWM = -mForward;
    } else if(mTurn == SAFESTOP) {
        int pForward = calcScurve(0.6, 100, 0, 1, false);

        rightPWM = -pForward;
        leftPWM =  -pForward;
    } else if(mTurn == BSAFESTOP) {
        int pForward = calcScurve(0.5, 100, 0, 1, false);

        rightPWM = pForward;
        leftPWM =  pForward;
    } else if(mTurn == STARTBACK) {
        int pForward = calcScurve(1.2, 100, 5, 1, true);
        
        rightPWM = pForward; //モーターにパワーを設定する
        leftPWM =  pForward; 
    } else if(mTurn == FWSTRAIGHT) {
        rightPWM = mForward;
        leftPWM = mForward;
    } else if(mTurn == ROLLLEFT) {
        rightPWM = 20;
        leftPWM = -20;
    } else if(mTurn == LINETRACE) {
        int pidVAL = mLineTracer->run();
        rightPWM = (mForward - pidVAL) * 0.6;
        leftPWM = (mForward + pidVAL) * 0.6;
    }
    //printf("%d, %d\n", rightPWM, leftPWM);
    mRightWheel.setPWM(rightPWM);
    mLeftWheel.setPWM(leftPWM);
    //ジャイロセンサーテスト用
    //printf("%d\n",ev3_gyro_sensor_get_angle(EV3_PORT_3)); 
}

/**
 * 走行に必要なものをリセットする
 */
void Walker::init() {
    // モータをリセットする
    mLeftWheel.reset();
    mRightWheel.reset();
}

/**
 * PWM値を設定する
 * @param forward 前進値
 * @param turn    旋回方向
 */
void Walker::setCommand(int forward, int turn) {
    mForward = forward;
    mTurn    = turn;
}

void Walker::setup() {
    printf("RESET!!!");
    prev_time_sum = 0;
    counter = 0;
}

/**
 * Sカーブの計算(加速時間、最大速度、最小速度、繰り返し回数(デフォは1回)、開始速度(trueでmin→max))
 */
int Walker::calcScurve(float AT, int Pmax, int Pmin, int repeat, bool accel) {
    float current_time_sum = 0;
    float time_diff;
    if (counter == 0) { // カウンターが0の場合、time_diffを0に設定する
        time_diff = 0;
    } else {
        time_diff = mSimpleTimer->getTimeDifference();// 時間の変化量を取得する
        time_diff /= 1000000.0; // マイクロ秒から秒に変換する
        float current_time_diff = time_diff;
        current_time_sum = prev_time_sum + current_time_diff; // 現在までの時間の合計を計算する
    }
    mSimpleTimer->saveCurrentTime(); // 現在の時間を保存する
    float fPow;

    if (current_time_sum >= AT * repeat) {
        double prev_time_sum = 0;
        fPow = Pmax;
    } else {
        if (accel) {
            fPow = (Pmax - Pmin) / 2 * (-cos(M_PI / AT * current_time_sum) + 1) + Pmin;
        } else {
            fPow = (Pmax - Pmin) / 2 * (cos(M_PI / AT * current_time_sum) + 1) + Pmin;
        }
    }

    int pForward = static_cast<int>(fPow); //floatの難しい計算の結果をintに変換する
    prev_time_sum = current_time_sum; //前回までの時間の合計を更新する
    counter++; // カウンターをインクリメントする
    return pForward;
}
