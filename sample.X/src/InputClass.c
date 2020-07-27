#include "InputClass.h"

// リセットスイッチ 状態保持変数
SW_PUSH_STATE_E IsPushedResetSW = false;
// スタートストップスイッチ 状態保持変数
SW_PUSH_STATE_E IsPushedStartStopSW = false;

SWState_t MinuteSW = {
    OFF,           // チャタフラグ
    LOW,           // 読み取った値
    RISING_STATE,  // チャタ状態
    0,             // チャタチェック用カウント変数
    OFF_STATE,     // スイッチ状態
    0,             // スイッチカウント
    OFF,           // スイッチタイミングフラグ
    0              // タイミングカウント
};

SWState_t SecondSW = {
    OFF,           // チャタフラグ
    LOW,           // 読み取った値
    RISING_STATE,  // チャタ状態
    0,             // チャタチェック用カウント変数
    OFF_STATE,     // スイッチ状態
    0,             // スイッチカウント
    OFF,           // スイッチタイミングフラグ
    0              // タイミングカウント
};

SWState_t StartStopSW = {
    OFF,           // チャタフラグ
    LOW,           // 読み取った値
    RISING_STATE,  // チャタ状態
    0,             // チャタチェック用カウント変数
    OFF_STATE,     // スイッチ状態
    0,             // スイッチカウント
    OFF,           // スイッチタイミングフラグ
    0              //タイミングカウント
};

// リセットスイッチ同時押し検知

void DetectResetSW(void) {
    if (MinuteSW.PushState == ON_STATE && SecondSW.PushState == ON_STATE) {
        // リセットの状態をON
        IsPushedResetSW = ON_STATE;
    } else {
        // リセットスイッチの状態をOFF
        IsPushedResetSW = OFF_STATE;
    }
}

// 長押し検知
// 入力：分、秒スイッチ

void DetectLongPushedSW(SWState_t *i_SW) {
    if (i_SW->SWCount >= 250) {
        // 2.5秒(250)以上
        // 長押し2段階状態をON
        i_SW->PushState = LONG_STG2_STATE;
    }

    if (i_SW->SWCount >= 100) {
        // 長押し1段階目状態をON
        i_SW->PushState = LONG_STG1_STATE;
    } else {
        // SWカウントが0の時
        // 1秒(100)未満の時

        // 何もしない
    }
}