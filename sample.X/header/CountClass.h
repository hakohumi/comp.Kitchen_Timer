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

// カウントダウン終了カウント
extern uint8_t CountDownEndCount;

void CountDown(void);

void StateTransferProcess(void);

// 分カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 99 以外であれば、エラーとして false を返す
void SetMinuteCount(uint8_t i_minute);

// 秒カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 59 以外であれば、エラーとして false を返す

void SetSecondCount(uint8_t i_second);

// 引数で指定された値を分カウントに加算する
void AddMinuteCount(uint8_t i_minute);
// 引数で指定された値を秒カウントに加算する
void AddSecondCount(uint8_t i_second);


extern uint8_t MinuteCountTime;
extern uint8_t SecondCountTime;

#ifdef __cplusplus
}
#endif

#endif /* COUNTCLASS_H */
