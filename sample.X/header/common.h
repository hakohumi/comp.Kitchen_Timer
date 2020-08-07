/*
 * File:   common.h
 * Author: fuminori.hakoishi
 *
 * Created on July 16, 2020, 2:20 PM
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdint.h>

#include "../mcc_generated_files/pin_manager.h"

// -------------------------------------

//#define DEBUG
// #define SIMULATER

// ChattaStateType
enum {
    RISING_STATE, ONGOING_STATE, FALLING_STATE
};

// KitchenTimerStateType

enum {
    COUNTTIME_SETTING_STATE, COUNTDOWN_ONGOING_STATE, COUNTDOWN_END_STATE, RESET_STATE
};

#define ON true
#define OFF false

#define LED_ON 0
#define LED_OFF 1

// SW割込みの許可・禁止
#define SW1_IOCInterruptEnable() \
    do {                         \
        IOCBNbits.IOCBN2 = 1;    \
    } while (0)
#define SW2_IOCInterruptEnable() \
    do {                         \
        IOCBNbits.IOCBN5 = 1;    \
    } while (0)
#define SW3_IOCInterruptEnable() \
    do {                         \
        IOCBNbits.IOCBN0 = 1;    \
    } while (0)

#define SW1_IOCInterruptDisable() \
    do {                          \
        IOCBNbits.IOCBN2 = 0;     \
    } while (0)
#define SW2_IOCInterruptDisable() \
    do {                          \
        IOCBNbits.IOCBN5 = 0;     \
    } while (0)
#define SW3_IOCInterruptDisable() \
    do {                          \
        IOCBNbits.IOCBN0 = 0;     \
    } while (0)

// スイッチからの入力
// LATからでもちゃんと
// リアルタイムで入力できるかテスト
// LATを使用すると、リードできない
#define SW1_Value IO_RB2_PORT
#define SW2_Value IO_RB5_PORT
#define SW3_Value IO_RB0_PORT
//#define SW1_Value IO_RB2_LAT
//#define SW2_Value IO_RB5_LAT
//#define SW3_Value IO_RB0_LAT

// デバッグ用
#define LED1 IO_RA0_LAT
#define LED2 IO_RA1_LAT
#define LED3 IO_RA2_LAT
#define LED4 IO_RA4_LAT

extern uint8_t KitchenTimerState;

// キッチンタイマー状態をリセットへ変更
void SetKitchenTimerStateToReset(void);
// キッチンタイマー状態をカウント設定状態へ変更
void SetKitchenTimerStateToSetting(void);
// キッチンタイマー状態をカウントダウン中へ変更
void SetKitchenTimerStateToGoing(void);
// キッチンタイマー状態をカウントダウン終了へ変更
void SetKitchenTimerStateToEnd(void);

#endif /* COMMON_H */
