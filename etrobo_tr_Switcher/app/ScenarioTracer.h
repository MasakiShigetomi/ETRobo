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
#include "LineMonitor.h"

class ScenarioTracer {
public:
    ScenarioTracer(Walker* walker,
                   LineTracer* lineTracer,
                   Scenario* scenario,
                   SimpleTimer* timer,
                   LineMonitor* linemonitor);

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
    SimpleTimer* mSimpleTimer;
    State mState;
    LineMonitor* mLineMonitor;
    bool redDetectorFlag;   //trueの時は赤色検知オン、falseの時は赤色検知オフ

    void initAction();
    void setCommand(SceneCommands command);
    void modeChangeAction();
    void execUndefined();
    void execInitial();
    void execWalking();
    //
    void setSpeed(SceneSpeeds speed);
};

#endif  // EV3_APP_SCENARIOTRACER_H_
