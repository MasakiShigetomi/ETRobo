#ifndef _RETURN_LINE_H_
#define _RETURN_LINE_H_

#include <stdio.h>
#include <stdlib.h>
#include "ev3api.h"
#include "Distance.h"
#include "Direction.h"
#include "LineTracer.h"
#include "ColorSensor.h" // <2>

/* ライントレースのエッジ定義 */
#define R_EDGE 0 // 左エッジ
#define L_EDGE 1 // 右エッジ

/* カラーセンサの反射光の白黒判断基準値 */
#define WHITE 40 //白値判断基準閾値
#define BLACK 10 //黒値判断基準閾値

#define TASK_INTERVAL 4.0 // タスク周期
class LineTracer;
class Distance;
class Direction;
class ReturnLine
{
public:
    /* 公開関数 */
    ReturnLine();
    void ReturnLine_init(void);                  // 初期化関数
    bool_t ReturnLine_do(bool_t edge);           // ライン復帰関数
    bool_t ReturnLine_serchLine(void);           // ラインを探索する関数（非公開処理）
    bool_t ReturnLine_setDirection(bool_t edge); // 車体の方位を調整する関数
    void ReturnLine_setDirection_init(void);     // 車体の方位を調整する　初期化関数
    bool_t ReturnLine_discernLine(void);         // ラインを検知する関数
    void ReturnLine_discernLine_init(void);      // ラインを検知する 初期化関数
    void testmethod(void);

private:
    /* ライン復帰の状態遷移 */
    enum ReturnLine_enum
    {
        SERCH_LINE_INIT,
        SERCH_LINE,
        SET_DIRECTION_INIT,
        SET_DIRECTION
    };

    /* ライン探索の状態遷移 */
    enum serchLine_enum
    {
        SERCH_L,
        BACK_L,
        SERCH_R,
        BACK_R
    };

    /* 車体方位調整の状態遷移 */
    enum setDirection_enum
    {
        TURN_L,
        TURN_R
    };

    /* ライン検知の状態遷移 */
    enum discernLine_enum
    {
        DISCERN_WHITE1,
        DISCERN_BLACK,
        DISCERN_WHITE2
    };
    enum serchLine_Action_enum
    {               // ライン探索時の動作状態
        SERCH_TURN, // 探索方向に旋回
        SERCH_MOVE  // 目標位置まで探索しながら移動
    };
    ReturnLine_enum ReturnLine_state;
    serchLine_enum serchLine_state;
    setDirection_enum setDirection_state;
    discernLine_enum discernLine_state;
    serchLine_Action_enum serchLine_Action_state;
    ColorSensor colorSensor; // <3>
};

#endif
