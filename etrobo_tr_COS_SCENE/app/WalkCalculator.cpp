#include <stdlib.h>
#include "Walker.h"
#include "ev3api.h"
#include <time.h>
#include <cmath>
#include "Clock.h"

float pForward;
static double prev_time_sum = 0; //経過時間の合計

static int counter = 0;

WalkCalculator::WalkCalculator(SimpleTimer* WTimer)
    : mSimpleTimer(WTimer) {
    ev3api::Clock* clock = new ev3api::Clock();

    srand(clock->now());  // 乱数をリセットする

    delete clock;
}


/**
 * Sカーブの計算(加速→通常走行)
 */
int WalkCalculator::calcScurveAccel(float AT, int Pmax, int Pmin) {
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

        if (current_time_sum >= AT) {
            double prev_time_sum = 0;
            fPow = Pmax;
        } else {
            fPow = (Pmax - Pmin) / 2 * (-cos(M_PI / AT * current_time_sum) + 1) + Pmin; //難しい計算する
        }

        int pForward = static_cast<int>(fPow); //floatの難しい計算の結果をintに変換する
        prev_time_sum = current_time_sum; //前回までの時間の合計を更新する
        counter++; // カウンターをインクリメントする
        return pForward;
}


/**
 * Sカーブの計算(Pmax→減速)
 */
int WalkCalculator::calcScurveDecel(float AT, int Pmax, int Pmin) {
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

        if (current_time_sum >= AT) {
            double prev_time_sum = 0;
            fPow = Pmin;
        } else {
            fPow = (Pmax - Pmin) / 2 * (cos(M_PI / AT * current_time_sum) + 1) + Pmin; //難しい計算する
        }

        int pForward = static_cast<int>(fPow); //floatの難しい計算の結果をintに変換する
        prev_time_sum = current_time_sum; //前回までの時間の合計を更新する
        counter++; // カウンターをインクリメントする
        return pForward;
}


/**
 * Sカーブの計算(減速→加速(カーブ用))
 */
int WalkCalculator::calcScurveCurve(float AT, int Pmax, int Pmin) {
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

        if (current_time_sum >= AT) {
            double prev_time_sum = 0;
            fPow = Pmin;
        } else {
            fPow = (Pmax - Pmin) / 2 * (cos(M_PI / AT * current_time_sum) + 1) + Pmin; //難しい計算する
        }

        int pForward = static_cast<int>(fPow); //floatの難しい計算の結果をintに変換する
        prev_time_sum = current_time_sum; //前回までの時間の合計を更新する
        counter++; // カウンターをインクリメントする
        return pForward;
}

void WalkCalculator::setup() {
    printf("RESET!!!");
    prev_time_sum = 0;
    counter = 0;
}