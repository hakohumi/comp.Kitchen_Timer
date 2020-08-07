#ifndef INPUTCLASS_H
#define INPUTCLASS_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

// SW PushState
enum {
    OFF_STATE,
    ON_STATE,
    LONG_STG1_STATE,
    LONG_STG2_STATE,
};

typedef struct {
    bool ChattaFlg;       // チャタフラグ
    bool ReadValue;       // 読み取った値
    uint8_t ChattaState;  // チャタ状態
    uint8_t CheckCount;   // チャタチェック用カウント変数
    uint8_t PushState;    // スイッチ状態
    uint8_t SWCount;      // スイッチカウント
    bool TimingFlag;      // スイッチタイミングフラグ
    uint8_t TimingCount;  // タイミングカウント
} SWState_t;

// 各スイッチの状態保持用
extern SWState_t MinuteSW;
extern SWState_t SecondSW;
extern SWState_t StartStopSW;  // RB0

// リセットスイッチが押されているかを保持する
extern uint8_t IsPushedResetSW;

void InputProcess(void);

// inline void ClrResetSW(void);

#endif /* INPUTCLASS_H */
