/******************************************************************************
 *  RandomWalker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class RandomWalker
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include <stdlib.h>
#include "Clock.h"
#include "RandomWalker.h"

// 定数宣言
const int RandomWalker::MIN_TIME = 1000 * 1000;    // 切り替え時間の最小値
const int RandomWalker::MAX_TIME = 2000 * 1000;   // 切り替え時間の最大値

/**
 * コンストラクタ
 * @param lineTracer      ライントレーサ
 * @param scenarioTracer  シナリオトレーサ
 * @param starter         スタータ  
 * @param simpleTimer     タイマ
 */
RandomWalker::RandomWalker(LineTracer* lineTracer,
                 LineMonitor* lineMonitor,
                 ScenarioTracer* scenarioTracer,
                 const Starter* starter,
                 SimpleTimer* simpleTimer,
                 DistanceTracker* distanceTracker)
    : mLineTracer(lineTracer),
      mLineMonitor(lineMonitor),
      mScenarioTracer(scenarioTracer),
      mStarter(starter),
      mSimpleTimer(simpleTimer),
      mDistanceTracker(distanceTracker),
      
      mState(UNDEFINED),
      mTransitionCount(0) {
    ev3api::Clock* clock = new ev3api::Clock();

    srand(clock->now());  // 乱数をリセットする

    delete clock;
}

/**
 * ランダム走行する
 */
void RandomWalker::run() {
    switch (mState) {
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
    case BROCK_CARRY:
        execBrockCarry();
        break;
    default:
        break;
    }
}

/**
 * 乱数を取得する
 * @retrun 乱数
 */
int RandomWalker::getRandomTime() {
    return MIN_TIME +
        static_cast<int>(static_cast<double>(rand()) *
                         (MAX_TIME - MIN_TIME + 1.0) / (1.0 + RAND_MAX));
}

/**
 * シーン変更処理
 */
void RandomWalker::modeChangeAction() {
    mSimpleTimer->setTime(getRandomTime());
    mSimpleTimer->start();
}

/**
 * 未定義状態の処理
 */
void RandomWalker::execUndefined() {
    mState = WAITING_FOR_START;
}

/**
 * 開始待ち状態の処理
 */
void RandomWalker::execWaitingForStart() {
    if (mStarter->isPushed()) {
        mState = LINE_TRACING;

        modeChangeAction();
    }
}

void RandomWalker::execLineTracing() {
    // ReachedDistanceの引数を変更する
    if (mTransitionCount == 0) {
        mDistanceTracker->SetDistance(14690);//ライントレース終了距離
    }
    mLineTracer->run();
    printf("%d\n", mDistanceTracker->CountDistance());
    //printf("%d\n",mTransitionCount);
     
    if (mDistanceTracker->ReachedDistance()) {
        mState = SCENARIO_TRACING;
        mTransitionCount++;  // 状態遷移の回数をインクリメント
        mSimpleTimer->setTime(1650000);//1.7秒間タイマーセット
        mSimpleTimer->start();
    }
    if (mLineMonitor->detectRed()) {
        mState = BROCK_CARRY;
        mSimpleTimer->setTime(1000000);
        mSimpleTimer->start();
    }
}

void RandomWalker::execScenarioTracing() {
    mScenarioTracer->run();//セットした時間低速直進

    if (mSimpleTimer->isTimedOut()) {
        mSimpleTimer->stop();

        mState = LINE_TRACING;

        modeChangeAction();
    }
}

void RandomWalker::execBrockCarry() {
    mScenarioTracer->run();
    if (mSimpleTimer->isTimedOut()) {
    mSimpleTimer->stop();
    }
}