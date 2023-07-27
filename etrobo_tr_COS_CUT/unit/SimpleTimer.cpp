/******************************************************************************
 *  SimpleTimer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class SimpleTimer
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "SimpleTimer.h"

/**
 * コンストラクタ
 * @param clock クロック
 */
SimpleTimer::SimpleTimer(const ev3api::Clock& clock)
    : mClock(clock),
      mDelayMs(0),
      mStartTime(0),
      mTargetTime(0),
      mSavedTime(0),
      mDiffTime(0) {
}

/**
 * タイマ満期時間設定
 * @param delay 満期時間[ms]
 */
void SimpleTimer::setTime(uint32_t delay) {
    mDelayMs = delay;
}

/**
 * タイマ開始
 */
void SimpleTimer::start() {
    mStartTime  = mClock.now();
    mTargetTime = mStartTime + mDelayMs;
}

/**
 * タイマ停止
 */
void SimpleTimer::stop() {
    mStartTime  = 0;
    mTargetTime = 0;
}

void SimpleTimer::now() {
    printf("%.2f\n",mClock.now());
}

/**
 * タイマ満期したか否か
 * @retval true  タイマ満期した
 * @retval false タイマ満期していない
 */
bool SimpleTimer::isTimedOut() const {
    return (mClock.now() >= mTargetTime) ? true : false;
}

/**
 * タイマ開始したか否か
 * @retval true  タイマ開始した
 * @retval false タイマ開始していない
 */
bool SimpleTimer::isStarted() const {
    return (mStartTime == 0) ? false : true;
}

/**
 * 現在の時間を保存する
 */
void SimpleTimer::saveCurrentTime() {
    mSavedTime = mClock.now();
}

/**
 * 前回保存した時間と現在の時間の差分を取得する
 * @return 時間の差分[ms]
 */
uint32_t SimpleTimer::getTimeDifference() {
    uint32_t DiffTime = mClock.now() - mSavedTime;
    return DiffTime;
}