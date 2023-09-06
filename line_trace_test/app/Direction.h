#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "Distance.h"
#include "ReturnLine.h"

class Distance;
class Direction
{
public:
    /* 初期化 */
    void Direction_init();

    /* 方位を取得(右旋回が正転) */
    float Direction_getDirection();

    // 方位を更新
    void Direction_update();
    // float direction;
};

#endif
