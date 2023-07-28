#ifndef EV3_APP_WALKCALCULATOR_H_
#define EV3_APP_WALKCALCULATOR_H_

#include "SimpleTimer.h"
#include "ev3api.h"


class WalkCalculator {
public:
    WalkCalculator(SimpleTimer* WTimer);

    int calcScurveAccel(float AT, int Pmax, int Pmin);
    int calcScurveDecel(float AT, int Pmax, int Pmin);
    int calcScurveCurve(float AT, int Pmax, int Pmin);
    void setup();

private:
    SimpleTimer* mSimpleTimer;
};

#endif  // EV3_APP_WALKCALCULATOR_H_