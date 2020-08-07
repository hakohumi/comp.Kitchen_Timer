#include "InputClass.h"

// リセットスイッチ 状態保持変数
uint8_t IsPushedResetSW = OFF_STATE;

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

static void detectResetSW(void);
static void detectLongPushedSW(SWState_t *i_SW);

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

static void detectResetSW(void) {
    if (MinuteSW.PushState >= ON_STATE && SecondSW.PushState >= ON_STATE) {
        // リセットの状態をON
        IsPushedResetSW = ON_STATE;
    } else {
        // リセットスイッチの状態をOFF
        IsPushedResetSW = OFF_STATE;
    }
}

// inline void ClrResetSW(void) {
//     IsPushedResetSW = false;
// }

// 長押し検知
// 入力：分、秒スイッチ

static void detectLongPushedSW(SWState_t *i_SW) {
    if (i_SW->SWCount >= 250) {
        // 2.5秒(250)以上
        // 長押し2段階状態をON
        i_SW->PushState = LONG_STG2_STATE;
    } else if (i_SW->SWCount >= 100) {
        // 長押し1段階目状態をON
        i_SW->PushState = LONG_STG1_STATE;
    } else {
        // SWカウントが0の時
        // 1秒(100)未満の時

        // 何もしない
    }
}