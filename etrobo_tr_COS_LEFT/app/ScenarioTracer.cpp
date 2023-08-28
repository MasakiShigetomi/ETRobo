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
      printf("GO_FORWARD\n");
    } else if (command == S_FORWARD) {
      turn = Walker::SFORWARD;
      printf("S_FORWARD\n");
    } else if (command == GO_BACK) {
      turn = Walker::BACKWARD;
      printf("GO_BACK\n");
    } else if (command == S_BACK) {
      turn = Walker::SBACKWARD;
      printf("S_BACK\n");
    } else if (command == TURN_RIGHT) {
      turn = Walker::RIGHT;
      printf("TURN_RIGHT\n");
    } else if (command == S_RIGHT) {
      turn = Walker::SRIGHT;
      printf("S_RIGHT\n");
    } else if (command == BACK_RIGHT) {
      turn = Walker::BACKRIGHT;
      printf("BACK_RIGHT\n");
    } else if (command == S_BACKRIGHT) {
      turn = Walker::SBACKRIGHT;
      printf("S_BACKRIGHT\n");
    } else if (command == TURN_LEFT) {
      turn = Walker::LEFT;
      printf("TURN_LEFT\n");
    } else if (command == S_LEFT) {
      turn = Walker::SLEFT;
      printf("S_LEFT\n");
    } else if (command == BACK_LEFT) {
      turn = Walker::BACKLEFT;
      printf("BACK_LEFT\n");
    } else if (command == S_BACKLEFT) {
      turn = Walker::SBACKLEFT;
      printf("S_BACKLEFT\n");
    } else if (command == ROLL_RIGHT) {
      turn = Walker::ROLLRIGHT;
      printf("ROLL_RIGHT\n");
    } else if (command == ROLL_LEFT) {
      turn = Walker::ROLLLEFT;
      printf("ROLL_LEFT\n");
    } else if (command == STOP_HERE) {
      turn = Walker::STOP;
      printf("STOP_HERE\n");
    } else if (command == S_STOP) {
      turn = Walker::SSTOP;
      printf("S_STOP\n");
    } else if (command == B_ADJUST) {
      turn = Walker::BACKADJUST;
      printf("B_ADJUST\n");
    } else if (command == LINE_TRACE) {
      turn = Walker::LINETRACE;
      printf("LINE_TRACE\n");
    }

    mWalker->setCommand(turn);
}

void ScenarioTracer::setTask(SceneTasks taskcom) {
    int task = 0;
    if (taskcom == TIME_OUT) {
      task = SceneChanger::TIMEOUT;
      printf("TIMETASK_COUNTING\n");
    } else if(taskcom == LOOK_GREEN) {
      task = SceneChanger::LOOKGREEN;
      printf("COLORTASK_GREEN\n");
    } else if(taskcom == LOOK_BLACK) {
      task = SceneChanger::LOOKBLACK;
      printf("COLORTASK_BLACK\n");
    } else if(taskcom == LOOK_WHITE) {
      task = SceneChanger::LOOKWHITE;
      printf("COLORTASK_WHITE\n");
    } else if(taskcom == LOOK_RED) {
      task = SceneChanger::LOOKRED;
      printf("COLORTASK_RED\n");
    }

    mSceneChanger->setTask(task);
}

//
void ScenarioTracer::setSpeed(SceneSpeeds speed) {
    int forward = -2;

    if (speed == LOW) {
      forward = Walker::LOW;
    } else if (speed == MID) {
      forward = Walker::MID;
    } else if (speed == HIGH) {
      forward = Walker::HIGH;
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
