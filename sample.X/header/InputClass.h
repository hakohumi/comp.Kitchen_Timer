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
    SHORT_PUSH_STATE LONG_STG1_STATE,
    LONG_STG2_STATE,
} SW_PUSH_STATE_E;

// SW PushState

typedef struct {
    bool ChattaFlg;
    bool ReadValue;
    CHATTA_STATE_E ChattaState;
    uint8_t CheckCount;
    SW_PUSH_STATE_E PushState;
    uint8_t SWCount;
    bool TimingFlag;
} SWState_t;

// ResetSwitch PushState

typedef struct {
    bool PushState;
} ResetSWState_t;

// 同時押し検知
void DetectResetSW(void);

// 長押し検知
// 分、秒
void DetectLongPushedSW(SWState_t *i_SW);

extern SWState_t MinuteSW;     // RB2
extern SWState_t SecondSW;     // RB5
extern SWState_t StartStopSW;  // RB0

// リセットスイッチが押されているかを保持する
extern bool IsPushedResetSW;

#ifdef __cplusplus
}
#endif

#endif /* INPUTCLASS_H */
