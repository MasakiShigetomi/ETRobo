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
                               SimpleTimer* timer,
                               LineMonitor* linemonitor)
    : mWalker(walker),
      mLineTracer(lineTracer),
      mScenario(scenario),
      mSimpleTimer(timer),
      mState(UNDEFINED),
      mLineMonitor(linemonitor),
      redDetectorFlag(true) {
}

//シーン変更回数
int cnt = 0;

//シーン変更既定回数
int redDetectorFlagOn = 1;

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
    
    if (command == GO_STRAIGHT) {
      turn = Walker::STRAIGHT;
      printf("GO_STRAIGHT");
    } else if (command == TURN_LEFT) {
      turn = Walker::LEFT;
      printf("TURN_LEFT");
    } else if (command == TURN_RIGHT_S) {
      turn = Walker::RIGHTS;
      printf("TURN_RIGHT_S");
    } else if (command == STOP_HERE) {
      turn = Walker::STOP;
      printf("STOP_HERE");
    } else if (command == BACK_STRAIGHT) {
      turn = Walker::BACK;
      printf("BACK_STRAIGHT");
    } else if (command == BACK_RIGHT) {
      turn = Walker::BACKRIGHT;
      printf("BACK_RIGHT");
    } else if (command == BACK_LEFT) {
      turn = Walker::BACKLEFT;
      printf("BACK_LEFT");
    } else if (command == TURN_CLOCKWISE) {
      turn = Walker::CLOCKWISE;
      printf("TURN_CLOCKWISE");
    } else if (command == ANTI_CLOCKWISE) {
      turn = Walker::ACLOCKWISE;
      printf("ANTI_CLOCKWISE");
    } else if (command == LINE_TRACING) {
      turn = Walker::LINETRACE;
      printf("LINE_TRACING");
    }else if (command == GO_STRAIGHT_S) {
      turn = Walker::STRAIGHTS;
      printf("GO_STRAIGHT_S");
    } else if (command == TURN_RIGHT) {
      turn = Walker::RIGHT;
      printf("TURN_RIGHT");
    }

    mWalker->setCommand(turn);
  }

//
void ScenarioTracer::setSpeed(SceneSpeeds speed) {
    int forward = -2;

    if (speed == LOW) {
      forward = Walker::LOW;
      printf("LOW");
    } else if (speed == MID) {
      forward = Walker::MID;
      printf("MID");
    } else if (speed == HIGH) {
      forward = Walker::HIGH;
      printf("HIGH");
    }

    mWalker->setSpeed(forward);
}

/**
 * シーン変更処理
 */
void ScenarioTracer::modeChangeAction() {   //シーンを進め、Walkerクラスに現在シーンを設定する
    mScenario->next();  //次のシーンに進む

    printf("%s", (redDetectorFlag)? "TRUE\n" : "FALSE\n");
    if (cnt == redDetectorFlagOn) {
      redDetectorFlag = true;
      cnt++;
    }
    if (!redDetectorFlag) {
      cnt++;
    }


    SceneCommands command = mScenario->currentSceneCommand();   // 現在シーンコマンド取得
    setCommand(command);
    mWalker->setup();
    mSimpleTimer->setTime(mScenario->currentSceneTime());
    mSimpleTimer->start();
    SceneSpeeds speed = mScenario->currentSceneSpeed();
    setSpeed(speed);
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

    if (redDetectorFlag) {
      if (mLineMonitor->redDetector()) {
        modeChangeAction();
        redDetectorFlag = false;
      }
    }
    if (mSimpleTimer->isTimedOut()) {   // イベントチェック
        mSimpleTimer->stop();   // exit アクション

        modeChangeAction();    // entry アクション(WALKING)
    }
}
