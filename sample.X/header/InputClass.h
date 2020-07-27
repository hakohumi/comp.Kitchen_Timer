#ifndef INPUTCLASS_H
#define INPUTCLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

typedef enum {
    OFF_STATE,
    ON_STATE,
    SHORT_PUSH_STATE = 1,
    LONG_STG1_STATE,
    LONG_STG2_STATE,
} SW_PUSH_STATE_E;

// SW PushState

typedef struct {
    bool ChattaFlg;              // チャタフラグ
    bool ReadValue;              // 読み取った値
    CHATTA_STATE_E ChattaState;  // チャタ状態
    uint8_t CheckCount;          // チャタチェック用カウント変数
    SW_PUSH_STATE_E PushState;   // スイッチ状態
    uint8_t SWCount;             // スイッチカウント
    bool TimingFlag;             // スイッチタイミングフラグ
} SWState_t;

// ResetSwitch PushState

typedef struct {
    SW_PUSH_STATE_E PushState;
} ResetSWState_t;

// 各スイッチの状態保持用
extern SWState_t MinuteSW;
extern SWState_t SecondSW;
extern SWState_t StartStopSW;  // RB0

// リセットスイッチが押されているかを保持する
extern SW_PUSH_STATE_E IsPushedResetSW;

void DetectResetSW(void);
void DetectLongPushedSW(SWState_t *i_SW);

#ifdef __cplusplus
}
#endif

#endif /* INPUTCLASS_H */
