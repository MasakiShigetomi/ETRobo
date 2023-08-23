/******************************************************************************
 *  ScenarioTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class ScenarioTracer
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_SCENARIOTRACER_H_
#define EV3_APP_SCENARIOTRACER_H_

#include "Walker.h"
#include "LineTracer.h"
#include "SimpleTimer.h"
#include "Scenario.h"
#include "SceneChanger.h"

class ScenarioTracer {
public:
    ScenarioTracer(Walker* walker,
                   LineTracer* lineTracer,
                   Scenario* scenario,
                   SceneChanger* sceneChanger,
                   SimpleTimer* timer);

    void run();

private:
    enum State {
        UNDEFINED,
        INITIAL,
        WALKING
    };

    Walker* mWalker;
    LineTracer* mLineTracer;
    Scenario* mScenario;
    SceneChanger* mSceneChanger;
    SimpleTimer* mSimpleTimer;
    State mState;

    void initAction();
    void setCommand(SceneCommands command);
    void setTask(SceneTasks taskcom);
    void setSpeed(SceneSpeeds speed);
    void modeChangeAction();
    void execUndefined();
    void execInitial();
    void execWalking();
};

#endif  // EV3_APP_SCENARIOTRACER_H_
