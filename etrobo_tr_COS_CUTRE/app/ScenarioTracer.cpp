/******************************************************************************
 *  ScenarioTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class ScenarioTracer
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "ScenarioTracer.h"

/**
 * コンストラクタ
 * @param walker          走行
 * @param scenario        シナリオ
 * @param timer           タイマ
 */
ScenarioTracer::ScenarioTracer(Walker* walker,
                               LineTracer* lineTracer,
                               Scenario* scenario,
                               SimpleTimer* timer)
    : mWalker(walker),
      mLineTracer(lineTracer),
      mScenario(scenario),
      mSimpleTimer(timer),
      mState(UNDEFINED) {
}

/**
 * シナリオトレースする
 */
void ScenarioTracer::run() {
    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case INITIAL:
        execInitial();
        break;
    case WALKING:
        execWalking();
        break;
    default:
        break;
    }
}

/**
 * シナリオトレース初期化処理
 */
void ScenarioTracer::initAction() {
    mWalker->init();
}

/**
 * トレースコマンド設定
 * @param command 走行向き
 */
void ScenarioTracer::setCommand(SceneCommands command) {
    int turn = -1;
    if (command == LINE_TRACING) {
      mLineTracer->run();
      } else {
      if (command == GO_STRAIGHT) {
        turn = Walker::STRAIGHT;
      } else if (command == TURN_LEFT) {
        turn = Walker::LEFT;
      } else if (command == TURN_RIGHT) {
        turn = Walker::RIGHT;
      } else if (command == STOP_HERE) {
        turn = Walker::STOP;
      } else if (command == BACK_STRAIGHT) {
        turn = Walker::BACK;
      } else if (command == BACK_RIGHT) {
        turn = Walker::BACKRIGHT;
      } else if (command == BACK_LEFT) {
        turn = Walker::BACKLEFT;
      } else if (command == TURN_CLOCKWISE) {
        turn = Walker::CLOCKWISE;
      } else if (command == ANTI_CLOCKWISE) {
        turn = Walker::ACLOCKWISE;
      } 

      mWalker->setCommand(Walker::HIGH,turn);
    }
}
/**
 * シーン変更処理
 */
void ScenarioTracer::modeChangeAction() {
    mScenario->next();
    
    SceneCommands command = mScenario->currentSceneCommand();
    setCommand(command);
    mWalker->setup();
    mSimpleTimer->setTime(mScenario->currentSceneTime());
    mSimpleTimer->start();
}

/**
 * 未定義状態の処理
 */
void ScenarioTracer::execUndefined() {
    mState = INITIAL;
}

/**
 * 初期状態の処理
 */
void ScenarioTracer::execInitial() {
    initAction();   // entry アクション(INITIAL)

    mState = WALKING;   // 状態遷移

    modeChangeAction();    // entry アクション(WALKING)
}

/**
 * 走行中状態の処理
 */
void ScenarioTracer::execWalking() {
    mWalker->run();             // do アクティビティ

    if (mSimpleTimer->isTimedOut()) {   // イベントチェック
        mSimpleTimer->stop();   // exit アクション

        modeChangeAction();    // entry アクション(WALKING)
    }
}
