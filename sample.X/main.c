/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules
  selected in the GUI. Generation Information : Product Revision  :  PIC10 /
  PIC12 / PIC16 / PIC18 MCUs - 1.81.3 Device            :  PIC16F1827 Driver
  Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software
   and any derivatives exclusively with Microchip products. It is your
   responsibility to comply with third party license terms applicable to your
   use of third party software (including open source software) that may
   accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
 */
#include "InputClass.h"
#include "LCDClass.h"
#include "common.h"
#include "mcc_generated_files/mcc.h"
#include "tmr1.h"

/*
                         Main application
 */

bool Timer500msFlag;
bool Timer10msFlag;

// キッチンタイマー状態 初期値:リセット状態
KITCHEN_TIMER_STATE_E KitchenTimerState = RESET_STATE;

// スイッチ状態検知
void InputProcess(void);
// 状態遷移処理
void StateTransferProcess(void);
// LCD更新・ブザー状態更新s
void OutputProcess(void);

void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

    // LCDの初期化
    InitLCD();

    while (1) {
        InputProcess();
        StateTransferProcess();
        OutputProcess();
    }
}

/*
スイッチ状態検知
 */
void InputProcess(void) {
    // リセットスイッチ同時押し検知
    DetectResetSW();

    // リセットスイッチの状態
    // OFF の場合
    if (!IsPushedResetSW) {
        // 分スイッチ長押し検知
        DetectLongPushedSW(&MinuteSW);
        // 秒スイッチ長押し検知
        DetectLongPushedSW(&SecondSW);
    }
}

/*
状態遷移処理
CoutClass
*/

void StateTransferProcess(void) {
    switch (KitchenTimerState) {
        case COUNTTIME_SETTING_STATE:
            break;
        case COUNTDOWN_RUNNING_STATE:
            break;
        case COUNTDOWN_END_STATE:
            break;
        case RESET_STATE:
            break;
    }
}

/*
LCD更新
ブザー状態更新
*/

void OutputProcess(void) {
    // UpdateLCD();
    // UpdateBuzzer();
}

/**
 End of File
 */
