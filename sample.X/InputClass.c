#include "InputClass.h"

// リセットスイッチ 状態保持変数
bool IsPushedResetSW = false;

extern SWState_t SW1;
extern SWState_t SW2;

// リセットスイッチ同時押し検知

void DetectResetSW() {
    if (SW1.PushState == ON && SW2.PushState == ON) {
        // リセットの状態をON
        IsPushedResetSW = ON;
    } else {
        // リセットスイッチの状態をOFF
        IsPushedResetSW = OFF;
    }
}

// 長押し検知
// 入力：分、秒スイッチ

void DetectLongPushedSW(SWState_t *i_SW) {
    if (i_SW->SWCount < 100) {
        // 1秒(100)未満
        // 長押し状態をOFF
        i_SW->PushState = OFF_STATE;
    } else if (i_SW->SWCount < 250) {
        // 長押し1段階目状態をON
        i_SW->PushState = STG1_STATE;
    } else {
        // 2.5秒(250)以上
        // 長押し2段階状態をON
        i_SW->PushState = STG2_STATE;
    }
}