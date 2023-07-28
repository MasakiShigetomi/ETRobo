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
const int Walker::CLOCKWISE  = 7;
const int Walker::ACLOCKWISE = 8;
const float T1 = 1.5;
const float T2 = 2.15;
const float T3 = 1.5;
const int Pmax = 100;
const int Pmin = 5;
const int CPmin = 55; //カーブ用の最小値

bool first_iteration = true;

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
Walker::Walker(ev3api::Motor& leftWheel,
                                 ev3api::Motor& rightWheel,
                                 WalkCalculator* walkCalculator)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mForward(LOW),
      mTurn(RIGHT),
      mWalkCalculator(walkCalculator) {
}

/**
 * 走行する
 */
void Walker::run() {
    // 左右モータに回転を指示する
    int rightPWM = 0;
    int leftPWM = 0;
    if(mTurn == STRAIGHT) {
        int pForward = mWalkCalculator->calcScurveAccel(T1, Pmax, Pmin);
        //printf("%d\n",pForward);//ログ出力
        rightPWM = pForward; //モーターにパワーを設定する
        leftPWM =  pForward;
        }
     else if(mTurn == RIGHT) {
        int pForward = mWalkCalculator->calcScurveCurve(T2, Pmax, CPmin);
        //printf("%d\n",pForward);//ログ出力
        rightPWM = pForward; //モーターにパワーを設定する
        leftPWM =  mForward;
    } else if(mTurn == LEFT) {
        rightPWM = mForward;
        leftPWM = 0;
    } else if(mTurn == STOP) {
        rightPWM = 0;
        leftPWM = 0;
    } else if(mTurn == BACK) {
        rightPWM = mForward;
        leftPWM = mForward;
    } else if(mTurn == BACKRIGHT) {
        rightPWM = -mForward;
        leftPWM = 0;
    } else if(mTurn == BACKLEFT) {
        rightPWM = 0;
        leftPWM = -mForward;
    } else if(mTurn == CLOCKWISE) {
        rightPWM = -mForward;
        leftPWM = mForward;
    } else if(mTurn == ACLOCKWISE) {
        rightPWM = mForward;
        leftPWM = -mForward;
    } else {
        rightPWM = mForward;
        leftPWM = mForward;
    }
   
    mRightWheel.setPWM(rightPWM);
    mLeftWheel.setPWM(leftPWM);
    printf("%d\n",ev3_gyro_sensor_get_angle(EV3_PORT_3));
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

