/******************************************************************************
 *  Scene.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Struct Scene
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_SCENE_H_
#define EV3_APP_SCENE_H_

enum SceneCommands {
    GO_FORWARD = 0,
    S_FORWARD,
    GO_BACK,
    S_BACK,
    TURN_RIGHT,
    S_RIGHT,
    BACK_RIGHT,
    S_BACKRIGHT,
    TURN_LEFT,
    S_LEFT,
    BACK_LEFT,
    S_BACKLEFT,
    ROLL_RIGHT,
    ROLL_LEFT,
    STOP_HERE,
    S_STOP,
    B_ADJUST,
    LINE_TRACE
};

enum SceneTasks {
    TIME_OUT = 0,
    LOOK_GREEN,
    LOOK_BLACK,
    LOOK_WHITE,
    LOOK_RED
};

enum SceneSpeeds {
    LOW,
    MID,
    HIGH
};

struct Scene {
    SceneCommands command;
    SceneTasks    taskcom;
    int           time;
    Scene*        next;
    SceneSpeeds   speed;
};

#endif  // EV3_APP_SCENE_H_
