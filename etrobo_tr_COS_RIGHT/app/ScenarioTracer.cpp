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
    } else if (command == SAFE_STOP) {
      turn = Walker::SAFESTOP;
    } else if (command == BSAFE_STOP) {
      turn = Walker::BSAFESTOP;
    } else if (command == START_BACK) {
      turn = Walker::STARTBACK; 
    } else if (command == FORWARD_STRAIGHT) {
      turn = Walker::FWSTRAIGHT;
    } else if (command == ROLL_LEFT) {
      turn = Walker::ROLLLEFT;
    } else if (command == LINE_TRACING) {
      turn = Walker::LINETRACE;
    }
    
    mWalker->setCommand(Walker::HIGH,turn);
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
    }

    mSceneChanger->setCommand(task);
}
    
/**
 * シーン変更処理
 */
void ScenarioTracer::modeChangeAction() {
  mScenario->next();
  SceneCommands command = mScenario->currentSceneCommand();
  SceneTasks taskcom = mScenario ->currentSceneTask();
  setCommand(command);
  setTask(taskcom);
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
