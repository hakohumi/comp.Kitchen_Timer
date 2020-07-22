#include "InputClass.h"

// リセットスイッチ 状態保持変数
bool IsPushedResetSW = false;

SWState_t MinuteSW = {
    OFF,           // ChattaFlg
    OFF,           // ReadValue
    RISING_STATE,  // ChattaState
    0,             // CheckCount
    OFF_STATE,     // PushState
    0,             // SWCount
    false          // TimingFlag
};

SWState_t SecondSW = {
    OFF,           // ChattaFlg
    OFF,           // ReadValue
    RISING_STATE,  // ChattaState
    0,             // CheckCount
    OFF_STATE,     // PushState
    0,             // SWCount
    false          // TimingFlag
};

SWState_t StartStopSW = {
    OFF,           // ChattaFlg
    OFF,           // ReadValue
    RISING_STATE,  // ChattaState
    0,             // CheckCount
    OFF_STATE,     // PushState
    0,             // SWCount
    false          // TimingFlag
};

/*
スイッチ状態検知
 */
void InputProcess(void) {
    // リセットスイッチ同時押し検知
    detectResetSW();

    // リセットスイッチの状態
    // OFF の場合
    if (!IsPushedResetSW) {
        // 分スイッチ長押し検知
        detectLongPushedSW(&MinuteSW);
        // 秒スイッチ長押し検知
        detectLongPushedSW(&SecondSW);
    }
}

// リセットスイッチ同時押し検知

void detectResetSW() {
    if (MinuteSW.PushState == ON && SecondSW.PushState == ON) {
        // リセットの状態をON
        IsPushedResetSW = ON;
    } else {
        // リセットスイッチの状態をOFF
        IsPushedResetSW = OFF;
    }
}

// 長押し検知
// 入力：分、秒スイッチ

void detectLongPushedSW(SWState_t *i_SW) {
    if (i_SW->SWCount >= 250) {
        // 2.5秒(250)以上
        // 長押し2段階状態をON
        i_SW->PushState = LONG_STG2_STATE;
    } else if (i_SW - SWCount >= 100) {
        // 長押し1段階目状態をON
        i_SW->PushState = lONG_STG1_STATE;
    } else {
        // SWカウントが0の時
        // 1秒(100)未満の時

        // 何もしない
    }
}