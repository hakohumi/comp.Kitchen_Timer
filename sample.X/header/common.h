/* 
 * File:   common.h
 * Author: fuminori.hakoishi
 *
 * Created on July 16, 2020, 2:20 PM
 */

#ifndef COMMON_H
#define	COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "pin_manager.h"


    // -------------------------------------

    typedef enum {
        RISING,
        ONGOING,
        FALLING
    } CHATTA_STATE_E;

    typedef enum {
        COUNTTIME_SETTING_STATE,
        COUNTDOWN_RUNNING_STATE,
        COUNTDOWN_END_STATE,
        RESET_STATE
    } KITCHEN_TIMER_STATE_E;

    typedef struct {
        bool ChattaFlg;
        bool ReadValue;
        CHATTA_STATE_E ChattaState;
        uint8_t CheckCount;
        bool State;
        uint8_t SWCount;
    } SWState_t;

#define ON true
#define OFF false

#define LED_ON 0
#define LED_OFF 1


    // SW割込みの許可・禁止
#define IOC_INT_SW1_ENABLE()          do { IOCBNbits.IOCBN2 = 1;} while(0)
#define IOC_INT_SW2_ENABLE()          do { IOCBNbits.IOCBN5 = 1;} while(0)
#define IOC_INT_SW3_ENABLE()          do { IOCBNbits.IOCBN0 = 1;} while(0)

#define IOC_INT_SW1_DISABLE()          do { IOCBNbits.IOCBN2 = 0;} while(0)
#define IOC_INT_SW2_DISABLE()          do { IOCBNbits.IOCBN5 = 0;} while(0)
#define IOC_INT_SW3_DISABLE()          do { IOCBNbits.IOCBN0 = 0;} while(0)

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
#define LED1    IO_RA0_LAT
#define LED2    IO_RA1_LAT
#define LED3    IO_RA2_LAT
#define LED4    IO_RA4_LAT

#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

