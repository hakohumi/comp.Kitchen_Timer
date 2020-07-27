/*
 * File:   CountClass.h
 * Author: fuminori.hakoishi
 *
 * Created on July 16, 2020, 2:00 PM
 */

#ifndef COUNTCLASS_H
#define COUNTCLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "common.h"

// カウント時間(分)
extern uint8_t MinuteCountTime;
// カウント時間(秒)
extern uint8_t SecondCountTime;

// カウントダウン終了カウント
extern uint8_t CountDownEndCount;

void StateTransferProcess(void);

// 分カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 99 以外であれば、エラーとして false を返す
bool SetCountMinute(uint8_t i_minute);

// 秒カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 59 以外であれば、エラーとして false を返す

bool SetCountSecond(uint8_t i_second);
#ifdef __cplusplus
}
#endif

#endif /* COUNTCLASS_H */