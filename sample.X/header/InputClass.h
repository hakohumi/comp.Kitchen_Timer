#ifndef INPUTCLASS_H
#define INPUTCLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>
#include "common.h"

<<<<<<< Updated upstream
    typedef enum {
        OFF_STATE,
        STG1_STATE,
        STG2_STATE,
    } LONG_PUSH_STATE_E;

    // SW PushState

    typedef struct {
        bool ChattaFlg;
        bool ReadValue;
        CHATTA_STATE_E ChattaState;
        uint8_t CheckCount;
        bool PushState;
        LONG_PUSH_STATE_E LongPushState;
        uint8_t SWCount;
    } SWState_t;

    // ResetSwitch PushState

    typedef struct {
        bool PushState;
    } ResetSWState_t;

    void InputProcess(void);
    // 同時押し検知
    void detectResetSW(void);

    // 長押し検知
    // 分、秒
    void detectLongPushedSW(SWState_t *i_SW);
=======
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
void detectResetSW(void);

// 長押し検知
// 分、秒
void detectLongPushedSW(SWState_t *i_SW);

extern SWState_t MinuteSW;     // RB2
extern SWState_t SecondSW;     // RB5
extern SWState_t StartStopSW;  // RB0
>>>>>>> Stashed changes

// リセットスイッチが押されているかを保持する
extern bool IsPushedResetSW;

#ifdef __cplusplus
}
#endif

#endif /* INPUTCLASS_H */
