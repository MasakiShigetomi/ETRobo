#include "Direction.h"
#include "ReturnLine.h"
#include "Distance.h"

Distance distance2;
#define TREAD 140 //車体トレッド幅(140mm)

static float direction = 0.0; //現在の方位
/* 初期化 */
void Direction::Direction_init()
{
    direction = 0.0;
}

/* 方位を取得(右旋回が正転) */
float Direction::Direction_getDirection()
{
    return direction;
}

/* 方位を更新 */
void Direction::Direction_update()
{
    //(360 / (2 * 円周率 * 車体トレッド幅)) * (右進行距離 - 左進行距離)
    direction += (360.0 / (2.0 * PI * TREAD)) * (distance2.Distance_getDistance4msLeft() - distance2.Distance_getDistance4msRight());
}