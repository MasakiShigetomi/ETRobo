#include "Distance.h"
#include "LineTracer.h"
#include "ReturnLine.h"
#include "Direction.h"

#define TIRE_DIAMETER 90.0 //タイヤ直径（90mm）

static float distance = 0.0;         //走行距離
static float distance4msL = 0.0;     //左タイヤの4ms間の距離
static float distance4msR = 0.0;     //右タイヤの4ms間の距離
static float pre_angleL, pre_angleR; // 左右モータ回転角度の過去値
static float total_distanceL = 0.0;  //右タイヤの総走行距離
static float total_distanceR = 0.0;  //右タイヤの総走行距離

Distance::Distance() : leftWheel(PORT_C), rightWheel(PORT_B), colorSensor(PORT_2)
{ // <2>
}

/* 初期化関数 */
void Distance::Distance_init()
{
    //各変数の値の初期化
    distance = 0.0;
    distance4msR = 0.0;
    distance4msL = 0.0;
    //モータ角度の過去値に現在値を代入
    pre_angleL = leftWheel.getCount();
    pre_angleR = rightWheel.getCount();
}

/* 距離更新（4ms間の移動距離を毎回加算している） */
void Distance::Distance_update()
{
    float cur_angleL = leftWheel.getCount();  //左モータ回転角度の現在値
    float cur_angleR = rightWheel.getCount(); //右モータ回転角度の現在値
    float distance4ms = 0.0;                  // 4msの距離

    // 4ms間の走行距離 = ((円周率 * タイヤの直径) / 360) * (モータ角度過去値　- モータ角度現在値)
    distance4msL = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleL - pre_angleL); // 4ms間の左モータ距離
    distance4msR = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleR - pre_angleR); // 4ms間の右モータ距離
    distance4ms = (distance4msL + distance4msR) / 2.0;                         //左右タイヤの走行距離を足して割る
    distance += distance4ms;
    //モータの回転角度の過去値を更新
    pre_angleL = cur_angleL;
    pre_angleR = cur_angleR;
    total_distanceL += distance4msL;
    total_distanceR += distance4msR;
}

/* 走行距離を取得 */
float Distance::Distance_getDistance()
{
    return distance;
}

/* 右タイヤの4ms間の距離を取得 */
float Distance::Distance_getDistance4msRight()
{
    return distance4msR;
}

/* 左タイヤの4ms間の距離を取得 */
float Distance::Distance_getDistance4msLeft()
{
    return distance4msL;
}

/* 左タイヤの総走行距離を取得 */
float Distance::Distance_getTotalDistanceL()
{
    return total_distanceL;
}

/* 右タイヤの総走行距離を取得 */
float Distance::Distance_getTotalDistanceR()
{
    return total_distanceR;
}

void Distance::testmethod()
{
    printf("Testing\n");
}