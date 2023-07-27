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
const int Walker::CLOCKWISE  = 7;
const int Walker::ACLOCKWISE = 8;
const float T1 = 1.5;
const float T2 = 2.15;
const float T3 = 1.5;
const int Pmax = 100;
const int Pmin = 5;
const int CPmin = 55; //カーブ用の最小値

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
                                 SimpleTimer* WTimer)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mForward(LOW),
      mTurn(RIGHT),
      mSimpleTimer(WTimer) {
    ev3api::Clock* clock = new ev3api::Clock();

    srand(clock->now());  // 乱数をリセットする

    delete clock;
}

/**
 * 走行する
 */
void Walker::run() {
    // 左右モータに回転を指示する
    int rightPWM = 0;
    int leftPWM = 0;
    if(mTurn == STRAIGHT) {

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

        if (current_time_sum >= T1) {
            double prev_time_sum = 0;
            fPow = Pmax;
        } else {
            fPow = (Pmax - Pmin) / 2 * (-cos(M_PI / T1 * current_time_sum) + 1) + Pmin; //難しい計算する
        }

        int pForward = static_cast<int>(fPow); //floatの難しい計算の結果をintに変換する
        //printf("%d\n",pForward);//ログ出力
        rightPWM = pForward; //モーターにパワーを設定する
        leftPWM =  pForward;
        prev_time_sum = current_time_sum; //前回までの時間の合計を更新する
        counter++; // カウンターをインクリメントする
        }
     else if(mTurn == RIGHT) {

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
        
        if (current_time_sum >= T2 * 2) {
            double prev_time_sum = 0;
            fPow = Pmax;
        } else {
            fPow = (Pmax - CPmin) / 2 * (cos(M_PI / T2 * current_time_sum) + 1) + CPmin; //難しい計算する
        }
        int pForward = static_cast<int>(fPow); //floatの難しい計算の結果をintに変換する
        //printf("%d\n",pForward);//ログ出力
        rightPWM = pForward; //モーターにパワーを設定する
        leftPWM =  mForward;
        prev_time_sum = current_time_sum; //前回までの時間の合計を更新する
        counter++; // カウンターをインクリメントする

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

void Walker::setup() {
    printf("RESET!!!");
    prev_time_sum = 0;
    counter = 0;
}
