/******************************************************************************
 *  RandomWalker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class RandomWalker
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include <stdlib.h>
#include "RandomWalker.h"


/**
 * コンストラクタ
 * @param lineTracer      ライントレーサ
 * @param scenarioTracer  シナリオトレーサ
 * @param starter         スタータ  
 * @param simpleTimer     タイマ
 */
//ディスタンストラッカーなど状使用しないものも宣言しています
RandomWalker::RandomWalker(LineTracer* lineTracer,
                           ScenarioTracer* scenarioTracer,
                           const Starter* starter,
                           DistanceTracker* distanceTracker)
    : mLineTracer(lineTracer),
      mScenarioTracer(scenarioTracer),
      mStarter(starter),
      mDistanceTracker(distanceTracker),
      mState(UNDEFINED),
      mTransitionCount(0),
      colorSensor(PORT_2) {

}

/**
 * スタートを検知し、シナリオ走行する
 */
void RandomWalker::run() {
    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case WAITING_FOR_START:
        execWaitingForStart();
        break;
    case SCENARIO_TRACING:
        execScenarioTracing();
        break;
    default:
        break;
    }
}

/**
 * 乱数を取得する　※不使用
 * @retrun 乱数
 */
int RandomWalker::getRandomTime() {
}

/**
 * シーン変更処理　※不使用
 */
void RandomWalker::modeChangeAction() {
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
        mState = SCENARIO_TRACING;

        modeChangeAction();
    }
}

/**
 * シナリオトレース開始
 */
void RandomWalker::execScenarioTracing() {

    mScenarioTracer->run();//シナリオに従い走行開始

}
