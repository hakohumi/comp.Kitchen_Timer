#ifndef INPUTCLASS_H
#define INPUTCLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>
#include "common.h"

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

#ifdef __cplusplus
}
#endif

#endif /* INPUTCLASS_H */
