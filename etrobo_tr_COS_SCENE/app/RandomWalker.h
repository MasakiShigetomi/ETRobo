/******************************************************************************
 *  RandomWalker.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class RandomWalker
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_RANDOMWALKER_H_
#define EV3_APP_RANDOMWALKER_H_

#include "Starter.h"
#include "DistanceTracker.h"
#include "LineTracer.h"
#include "ScenarioTracer.h"
#include "ColorSensor.h"

class RandomWalker {
public:
    RandomWalker(LineTracer* lineTracer,
                 ScenarioTracer* scenarioTracer,
                 const Starter* starter,
                 DistanceTracker* distanceTracker);

    void run();

private:
    enum State {
        UNDEFINED,
        WAITING_FOR_START,
        LINE_TRACING,
        SCENARIO_TRACING,
        BROCK_CARRY,
        STOP_HERE
    };

    int mTransitionCount; // 状態遷移の回数
    
    LineTracer* mLineTracer;
    ScenarioTracer* mScenarioTracer;
    const Starter* mStarter;
    DistanceTracker* mDistanceTracker;
    State mState;
    ColorSensor colorSensor;

    int getRandomTime();
    void modeChangeAction();
    void execUndefined();
    void execWaitingForStart();
    void execLineTracing();
    void execScenarioTracing();
    void execBrockCarry();
    void execStopHere();
};

#endif  // EV3_APP_RANDOMWALKER_H_
