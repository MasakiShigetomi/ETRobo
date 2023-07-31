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
    case STOP_HERE:
        execStopHere();
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
    //使わない
}

/**
 * シーン変更処理
 */
void RandomWalker::modeChangeAction() {
    //使わない
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
    mScenarioTracer->run();
    //printf("%d\n", mDistanceTracker->CountDistance());
    //printf("%d\n",mTransitionCount);
     
    if (mDistanceTracker->ReachedDistance()) {
        mState = SCENARIO_TRACING;
        mTransitionCount++;  // 状態遷移の回数をインクリメント
    }

    //赤検知→ブロック運び
    if (colorSensor.getColorNumber() == 5) {
        mState = BROCK_CARRY;
    }

}

void RandomWalker::execScenarioTracing() {
    mScenarioTracer->run();//セットした時間低速直進

        mState = LINE_TRACING;

}

void RandomWalker::execBrockCarry() {
    mScenarioTracer->run();//セットした時間低速直進
}

void RandomWalker::execStopHere() {

}