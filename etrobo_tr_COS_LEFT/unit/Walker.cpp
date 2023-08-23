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
const int Walker::MID = 50;    // 通常
const int Walker::HIGH   = 100;    // 高速

const int Walker::FORWARD = 0;
const int Walker::SFORWARD  = 1;    
const int Walker::BACKWARD  = 2;     
const int Walker::SBACKWARD   = 3;
const int Walker::RIGHT  = 4;
const int Walker::SRIGHT  = 5;
const int Walker::BACKRIGHT   = 6;
const int Walker::SBACKRIGHT  = 7;
const int Walker::LEFT = 8;
const int Walker::SLEFT = 9;
const int Walker::BACKLEFT = 10;
const int Walker::SBACKLEFT = 11;
const int Walker::ROLLRIGHT = 12;
const int Walker::ROLLLEFT  = 13;
const int Walker::STOP  = 14;
const int Walker::SAFESTOP  = 15;
const int Walker::BSAFESTOP  = 16;
const int Walker::LINETRACE  = 17;

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
    
    if(mTurn == FORWARD) {
        rightPWM = mForward; //モーターにパワーを設定する
        leftPWM =  mForward;

    } else if(mTurn == SFORWARD) {
        int pForward = calcScurve(1.2, mForward, 5, 1, true);
        rightPWM = pForward; 
        leftPWM =  pForward; 

    } else if(mTurn == BACKWARD) {
        rightPWM = -mForward;
        leftPWM =  -mForward; 

    } else if(mTurn == SBACKWARD) {
        int pForward = calcScurve(1.2, mForward, 5, 1, true);
        rightPWM = -pForward;
        leftPWM =  -pForward; 

    } else if(mTurn == RIGHT) {
        rightPWM = 0;
        leftPWM =  mForward;

    } else if(mTurn == SRIGHT) {
        int pForward = calcScurve(2, mForward, 0, 2, false);
        rightPWM = 0; 
        leftPWM =  pForward;

    } else if(mTurn == BACKRIGHT) {
        rightPWM = -mForward; 
        leftPWM =  0;
    
    } else if(mTurn == SBACKRIGHT) {
        int pForward = calcScurve(2.18, mForward, 52, 2, false);
        rightPWM = -pForward; 
        leftPWM =  mForward;

    } else if(mTurn == LEFT) {
        rightPWM = mForward; 
        leftPWM =  0;

    } else if(mTurn == SLEFT) {
        int pForward = calcScurve(2, mForward, 38, 2, false);
        rightPWM = mForward; 
        leftPWM =  pForward;
    
    } else if(mTurn == BACKLEFT) {
        rightPWM = 0; 
        leftPWM =  -mForward;
    
    } else if(mTurn == SBACKLEFT) {
        int pForward = calcScurve(2, mForward, 0, 2, false);
        rightPWM = 0; 
        leftPWM =  -pForward;

    } else if(mTurn == ROLLRIGHT) {
        rightPWM = -20;
        leftPWM =  20;

    } else if(mTurn == ROLLLEFT) {
        rightPWM = 20;
        leftPWM =  -20;

    } else if(mTurn == STOP) {
        rightPWM = 0;
        leftPWM =  0;

    } else if(mTurn == SAFESTOP) {
        int pForward = calcScurve(0.6, mForward, 0, 1, false);
        rightPWM = pForward;
        leftPWM =  pForward;

    } else if(mTurn == BSAFESTOP) {
        int pForward = calcScurve(0.6, mForward, 0, 1, false);
        rightPWM = -pForward;
        leftPWM =  -pForward;
    
    } else if(mTurn == LINETRACE) {
        int pidVAL = mLineTracer->run();
        rightPWM = (mForward - pidVAL) * 0.6;
        leftPWM =  (mForward + pidVAL) * 0.6;
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
void Walker::setCommand(int turn) {
    mTurn    = turn;
}

void  Walker::setSpeed(int forward) {
    mForward = forward;
}

//コメントアウト例
// ADD 2023/10/01 システム改修 Start
void Walker::setup() {
    printf("RESET!!!");
    prev_time_sum = 0;
    counter = 0;
}
// ADD 2023/10/01 システム改修 End

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
