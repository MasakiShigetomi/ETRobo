#ifndef _DISTANCE_H_
#define _DISTANCE_H_
#include "ev3api.h"
#include "Motor.h"       // <1>
#include "ColorSensor.h" // <2>
#include "util.h"        // <3>

/* 円周率 */
#define PI 3.14159265358

using namespace ev3api; // <4>
class LineTracer;
class Distance
{
public:
    Distance();
    /* 初期化関数 */
    void Distance_init();
    /* 距離を更新 */
    void Distance_update();
    /* 走行距離を取得 */
    float Distance_getDistance();
    /* 右タイヤの4ms間の距離を取得 */
    float Distance_getDistance4msRight();
    /* 左タイヤの4ms間の距離を取得 */
    float Distance_getDistance4msLeft();
    /* 左タイヤの総走行距離を取得 */
    float Distance_getTotalDistanceL();
    /* 右タイヤの総走行距離を取得 */
    float Distance_getTotalDistanceR();
    // テストメソッド
    void testmethod();

private:
    Motor leftWheel;
    Motor rightWheel;
    ColorSensor colorSensor; // <3>
    float calc_prop_value();
};
#endif