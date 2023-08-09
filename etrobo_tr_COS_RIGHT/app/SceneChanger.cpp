#include "SceneChanger.h"

const int SceneChanger::TIMEOUT = 0;
const int SceneChanger::LOOKGREEN = 1;
const int SceneChanger::LOOKBLACK = 2;
const int SceneChanger::LOOKWHITE = 3;

static bool Achieve = false;

SceneChanger::SceneChanger(Scenario* scenario,
                           LineMonitor* lineMonitor,
                            SimpleTimer* timer)
    :mScenario(scenario),
    mLineMonitor(lineMonitor),
    mSimpleTimer(timer),
    mTask(TIMEOUT) {
}

bool SceneChanger::judge() {
    if(mTask == TIMEOUT) {
        if (mSimpleTimer->isTimedOut()) {
            return true;
        } 
    } else if(mTask == LOOKGREEN) {
        if (mLineMonitor->getColor() == 3) {
            return true;
        }
    } else if(mTask == LOOKBLACK) {
        if (mLineMonitor->getColor() == 1) {
            return true;
        }
    } else if(mTask == LOOKWHITE) {
        if (mLineMonitor->getColor() == 6) {
            return true;
        }
    }
    return false;
}

void SceneChanger::setCommand(int task) {
    mTask = task;
}