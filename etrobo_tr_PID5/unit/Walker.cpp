/******************************************************************************
 *  Walker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Class Walker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Walker.h"

// 定数宣言
const int Walker::LOW    = 30;    // 低速
const int Walker::NORMAL = 50;    // 通常
const int Walker::HIGH   = 70;    // 高速

const int Walker::STRAIGHT = 0;
const int Walker::RIGHT  = 1;     // 左方向
const int Walker::LEFT   = 2;     // 右方向
const int Walker::STOP   = 3;
const int Walker::BACK   = 4;
const int Walker::BACKRIGHT  = 5;
const int Walker::BACKLEFT   = 6;
const int Walker::CLOCKWISE  = 7;
const int Walker::ACLOCKWISE = 8;

bool first_iteration = true;

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
Walker::Walker(ev3api::Motor& leftWheel,
                                 ev3api::Motor& rightWheel)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mForward(LOW),
      mTurn(RIGHT) {
}

/**
 * 走行する
 */
void Walker::run() {
    // 左右モータに回転を指示する
    int rightPWM = 0;
    int leftPWM = 0;
    
    if(mTurn == STRAIGHT) {
        const float Kp = 2.6;//1.8が最良
        int left_a = ev3_motor_get_counts(EV3_PORT_C );
        int right_a = ev3_motor_get_counts(EV3_PORT_B);
        printf("%d\n",left_a,right_a);
        int gap_a = Kp * (20 - (left_a - right_a));//20が最良
        //printf("%d\n",gap_a);
        rightPWM = mForward - gap_a;
        leftPWM = mForward + gap_a;
    } else if(mTurn == RIGHT) {
        rightPWM = 0;
        leftPWM = mForward;
    } else if(mTurn == LEFT) {
        rightPWM = mForward;
        leftPWM = 0;
    } else if(mTurn == STOP) {
        rightPWM = 0;
        leftPWM = 0;
    } else if(mTurn == BACK) {
        const float Kp = 1.8;//1.8が最良
        int left_a = ev3_motor_get_counts(EV3_PORT_C );
        int right_a = ev3_motor_get_counts(EV3_PORT_B);
        int gap_a = Kp * (10 - (left_a - right_a));//20が最良
        printf("%d\n",gap_a);
        rightPWM = -(mForward - gap_a);
        leftPWM = -(mForward + gap_a);
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
