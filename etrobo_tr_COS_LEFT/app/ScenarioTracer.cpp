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
                               SceneChanger* sceneChanger,
                               SimpleTimer* timer)
    : mWalker(walker),
      mLineTracer(lineTracer),
      mScenario(scenario),
      mSceneChanger(sceneChanger),
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
    
    if (command == GO_FORWARD) {
      turn = Walker::FORWARD;
    } else if (command == S_FORWARD) {
      turn = Walker::SFORWARD;
    } else if (command == GO_BACK) {
      turn = Walker::BACKWARD;
    } else if (command == S_BACK) {
      turn = Walker::SBACKWARD;
    } else if (command == TURN_RIGHT) {
      turn = Walker::RIGHT;
    } else if (command == S_RIGHT) {
      turn = Walker::SRIGHT;
    } else if (command == BACK_RIGHT) {
      turn = Walker::BACKRIGHT;
    } else if (command == S_BACKRIGHT) {
      turn = Walker::SBACKRIGHT;
    } else if (command == TURN_LEFT) {
      turn = Walker::LEFT;
    } else if (command == S_LEFT) {
      turn = Walker::SLEFT;
    } else if (command == BACK_LEFT) {
      turn = Walker::BACKLEFT;
    } else if (command == S_BACKLEFT) {
      turn = Walker::SBACKLEFT;
    } else if (command == ROLL_RIGHT) {
      turn = Walker::ROLLRIGHT;
    } else if (command == ROLL_LEFT) {
      turn = Walker::ROLLLEFT;
    } else if (command == STOP_HERE) {
      turn = Walker::STOP;
    } else if (command == SAFE_STOP) {
      turn = Walker::SAFESTOP;
    } else if (command == BSAFE_STOP) {
      turn = Walker::BSAFESTOP;
    } else if (command == LINE_TRACE) {
      turn = Walker::LINETRACE;
    }

    mWalker->setCommand(turn);
}

void ScenarioTracer::setTask(SceneTasks taskcom) {
    int task = 0;
    if (taskcom == TIME_OUT) {
      printf("TIMETASKREADING");
      task = SceneChanger::TIMEOUT;
    } else if(taskcom == LOOK_GREEN) {
      printf("COLORTASKREADING");
      task = SceneChanger::LOOKGREEN;
    } else if(taskcom == LOOK_BLACK) {
      printf("COLORTASKREADING");
      task = SceneChanger::LOOKBLACK;
    } else if(taskcom == LOOK_WHITE) {
      printf("COLORTASKREADING");
      task = SceneChanger::LOOKWHITE;
    } else if(taskcom == LOOK_RED) {
      printf("COLORTASKREADING");
      task = SceneChanger::LOOKRED;
    }

    mSceneChanger->setTask(task);
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
void ScenarioTracer::modeChangeAction() {
  mScenario->next();
  SceneCommands command = mScenario->currentSceneCommand();
  SceneTasks taskcom = mScenario->currentSceneTask();
  SceneSpeeds speed = mScenario->currentSceneSpeed();
  setCommand(command);
  setTask(taskcom);
  setSpeed(speed);
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
 * 走行中状態の処理 シーンチェンジャー判定
 */
void ScenarioTracer::execWalking() {
    mWalker->run();             // do アクティビティ

    if (mSceneChanger->judge() == true) {   // イベントチェック
        mSimpleTimer->stop();   // exit アクション

        modeChangeAction();    // entry アクション(WALKING)
    }
}
