#ifndef EV3_APP_SCENECHANGER_H_
#define EV3_APP_SCENECHANGER_H_

#include "SimpleTimer.h"
#include "Scenario.h"
#include "LineMonitor.h"

class SceneChanger {
public:
    static const int TIMEOUT;
    static const int LOOKGREEN;
    static const int LOOKBLACK;
    static const int LOOKWHITE;
    static const int LOOKRED;

    SceneChanger(Scenario* scenario,
                 LineMonitor* lineMonitor,
                 SimpleTimer* timer);

    bool judge();
    void setTask(int task);

private:

    Scenario* mScenario;
    LineMonitor* mLineMonitor;
    SimpleTimer* mSimpleTimer;
    int mTask;
};

#endif  // EV3_APP_SCENECHANGER_H_