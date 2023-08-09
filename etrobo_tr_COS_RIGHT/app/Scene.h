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
    GO_STRAIGHT = 0,
    TURN_LEFT,
    TURN_RIGHT,
    STOP_HERE,
    BACK_STRAIGHT,
    BACK_RIGHT,
    BACK_LEFT,
    SAFE_STOP,
    BSAFE_STOP,
    START_BACK,
    FORWARD_STRAIGHT,
    ROLL_LEFT,
    MASSUGU,
    LINE_TRACING
};

enum SceneTasks {
    TIME_OUT = 0,
    LOOK_GREEN,
    LOOK_BLACK,
    LOOK_WHITE,
};

struct Scene {
    SceneCommands command;
    SceneTasks taskcom;
    int           time;
    Scene*        next;
};

#endif  // EV3_APP_SCENE_H_
