﻿/******************************************************************************
 *  app.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "app.h"
#include "RandomWalker.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle=0;

// using宣言
using ev3api::GyroSensor; //qp
using ev3api::ColorSensor;
using ev3api::TouchSensor;
using ev3api::Motor;
using ev3api::Clock;


// Device objects
// オブジェクトを静的に確保する
GyroSensor  gGyroSensor(PORT_3);//qp
ColorSensor gColorSensor(PORT_2);
TouchSensor gTouchSensor(PORT_1);
Motor       gLeftWheel(PORT_C);
Motor       gRightWheel(PORT_B);
Clock       gClock;

// オブジェクトの定義
static Walker          *gWalker;
static LineMonitor     *gLineMonitor;
static Starter         *gStarter;
static SimpleTimer     *gScenarioTimer;
static SimpleTimer     *gWalkerTimer;
static SimpleTimer     *gKeeperTimer;
static LineTracer      *gLineTracer;
static Scenario        *gScenario;
static SceneChanger    *gSceneChanger;
static ScenarioTracer  *gScenarioTracer;
static RandomWalker    *gRandomWalker;
static DistanceTracker *gDistanceTracker;

// scene object
static Scene gScenes[] = {
    // { LINE_TRACING, TIME_OUT, 100000 * 1000, 0 }, //ライントレーステスト
    { GO_STRAIGHT, TIME_OUT, 6200 * 1000, 0 },  
    { TURN_RIGHT, TIME_OUT, 4360 * 1000, 0 },  //中間ゲート通過
    { BACK_STRAIGHT, TIME_OUT, 2000 * 1000, 0 }, 
    { SAFE_STOP, TIME_OUT, 600 * 1000, 0 },  //ポール倒し
    { START_BACK, TIME_OUT, 1200 * 1000, 0 },
    { BACK_RIGHT, TIME_OUT, 4000 * 1000, 0 },
    { FORWARD_STRAIGHT, LOOK_GREEN, 100000 * 1000, 0 }, //緑検知
    { FORWARD_STRAIGHT, LOOK_WHITE, 100000 * 1000, 0 }, //白検知
    { FORWARD_STRAIGHT, LOOK_BLACK, 100000 * 1000, 0 }, //黒検知
    { FORWARD_STRAIGHT, LOOK_WHITE, 100000 * 1000, 0 }, //白検知
    //{ BSAFE_STOP, TIME_OUT, 500 * 1000, 0 },
    { STOP_HERE, TIME_OUT, 400 * 1000, 0 }, //位置調整
    { SAFE_STOP, TIME_OUT, 600 * 1000, 0 },
    { ROLL_LEFT, LOOK_BLACK, 100000 * 1000, 0 }, //ライン上に合わせる
    { ROLL_LEFT, LOOK_WHITE, 100000 * 1000, 0 }, 
    { LINE_TRACING, TIME_OUT, 100000 * 1000, 0 } //ライントレース開始
};

/**
 * EV3システム生成
 */
static void user_system_create() {
    // [TODO] タッチセンサの初期化に2msのdelayがあるため、ここで待つ
    tslp_tsk(2U * 1000U);

    // オブジェクトの作成
    
    gDistanceTracker = new DistanceTracker(gLeftWheel,
                                        gRightWheel);                            
    gStarter         = new Starter(gTouchSensor);
    gLineMonitor     = new LineMonitor(gColorSensor);
    gScenarioTimer   = new SimpleTimer(gClock);
    gWalkerTimer     = new SimpleTimer(gClock);
    gKeeperTimer     = new SimpleTimer(gClock);
     
    gLineTracer      = new LineTracer(gLineMonitor,
                                      //gWalker,
                                      gDistanceTracker);
    gWalker          = new Walker(gLeftWheel,
                                  gRightWheel,
                                  gKeeperTimer,
                                  gLineTracer);                                 
    gScenario        = new Scenario(0);
    gSceneChanger    = new SceneChanger(gScenario,
                                        gLineMonitor,
                                        gScenarioTimer);
    gScenarioTracer  = new ScenarioTracer(gWalker,
                                          gLineTracer,
                                          gScenario,
                                          gSceneChanger,
                                          gScenarioTimer);
    gRandomWalker    = new RandomWalker(gLineTracer,
                                        gScenarioTracer,
                                        gStarter,
                                        gDistanceTracker);

    // シナリオを構築する
    for (uint32_t i = 0; i < (sizeof(gScenes)/sizeof(gScenes[0])); i++) {
        gScenario->add(&gScenes[i]);
    }
    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);
}

/**
 * EV3システム破棄
 */
static void user_system_destroy() {
    gLeftWheel.reset();
    gRightWheel.reset();

    delete gRandomWalker;
    delete gScenarioTracer;
    delete gSceneChanger;
    delete gScenario;
    delete gLineTracer;
    delete gWalkerTimer;
    delete gScenarioTimer;
    delete gKeeperTimer;
    delete gLineMonitor;
    delete gStarter;
    delete gWalker;
    delete gDistanceTracker;
}

/**
 * メインタスク
 */
void main_task(intptr_t unused) {
    user_system_create();  // センサやモータの初期化処理

    // 周期ハンドラ開始
    sta_cyc(CYC_TRACER);

    slp_tsk();  // バックボタンが押されるまで待つ

    // 周期ハンドラ停止
    stp_cyc(CYC_TRACER);

    user_system_destroy();  // 終了処理

    ext_tsk();
}

/**
 * ライントレースタスク
 */
void tracer_task(intptr_t exinf) {
    if (ev3_button_is_pressed(BACK_BUTTON)) {
        wup_tsk(MAIN_TASK);  // バックボタン押下
    } else {
        gRandomWalker->run();  // 走行
    }
    ext_tsk();
}
