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
#include "../mcc_generated_files/mcc.h"
#include "CountClass.h"
#include "InputClass.h"
#include "LCDClass.h"
#include "common.h"
#include "tmr1.h"

/*
                         Main application
 */

// キッチンタイマー状態 初期値:リセット状態
KITCHEN_TIMER_STATE_E KitchenTimerState = RESET_STATE;

// LCD更新・ブザー状態更新
inline void OutputProcess(void);

inline void updateLED(void);
inline void updateLCD(void);

// カウント時間を"00m00s"の形でLCDへ表示させる
void countTimeToLCD(uint8_t i_minute, uint8_t i_second);

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
        // updateLED();
        // updateLCD();
    }
}

/*
LCD更新
ブザー状態更新
 */

inline void OutputProcess(void) {
    updateLCD();
    updateLED();
    // UpdateBuzzer();
}

inline void updateLED() {
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;

    if (Is1sFlg) {
        LED4 = LED_ON;
    } else {
        LED4 = LED_OFF;
    }

    switch (KitchenTimerState) {
        case COUNTTIME_SETTING_STATE:
            LED1 = LED_ON;

            break;
        case COUNTDOWN_ONGOING_STATE:
            LED2 = LED_ON;

            break;
        case COUNTDOWN_END_STATE:
            LED3 = LED_ON;

            break;
        case RESET_STATE:
            LED1 = LED_ON;
            LED2 = LED_ON;
            LED3 = LED_ON;
            break;
    }
}

inline void updateLCD(void) {
    // 1行目 文字列バッファ
    uint8_t l_LCDBuf1[8] = "running";
    // 2行目 文字列バッファ
    uint8_t l_LCDBuf2[8] = "stt";

    // setting string
    uint8_t l_LCDstr1[] = "setting";
    uint8_t l_LCDstr2[] = "state";

    // UpdateLCDフラグがONか
    if (UpdateLCDFlg == ON) {
        // キッチンタイマーの状態は？
        switch (KitchenTimerState) {
                // カウント時間設定
            case COUNTTIME_SETTING_STATE:

                WriteUnitChar();

                countTimeToLCD(MinuteCountTime, SecondCountTime);

                break;

                // カウントダウン中
            case COUNTDOWN_ONGOING_STATE:
                // カウント時間をLCDバッファに格納
                SetPosLineLCD(false);
                countTimeToLCD(MinuteCountTime, SecondCountTime);

                // 1秒フラグがOFFか
                if (!Is1sFlg) {
                    // LCDバッファのmとsの文字を点滅
                    ClrUnitChar();
                } else {
                    WriteUnitChar();
                }
                break;

                // カウントダウン終了
            case COUNTDOWN_END_STATE:
                // カウント時間をLCDバッファに格納

                // 1秒フラグがOFFか
                if (!Is1sFlg) {
                    // LCDバッファの文字を点滅
                }

                break;

                // リセット状態 ありえない
            case RESET_STATE:

                break;

                // その他の状態 ありえない
            default:
                break;
        }
        // LCDバッファの値をLCDへ表示
        // UpdateLCDフラグをOFFにする
        SetUpdateLCDFlgOFF();
    }
}

// カウント時間をLCDへ書き込む
// 2行目の真ん中へ書く

void countTimeToLCD(uint8_t i_minute, uint8_t i_second) {
    uint8_t i_str[8];

    i_str[3] = 'm';
    i_str[6] = 's';

    // -------------------------------------------------------
    // sprintfは容量が大きいため、後に別の方法で実装する
    // -------------------------------------------------------
    // 分を変換
    utoa(i_minute, &i_str[1], 10);
    // 秒を変換
    utoa(i_second, &i_str[4], 10);
    // -------------------------------------------------------

    // 書き込み位置を2行目へセット
    SetPosLineLCD(true);
    // 1行書き込む
    Write1LineToLCD(i_str, 8);
}

// キッチンタイマー状態をリセットへ変更

void SetKitchenTimerStateToReset(void) { KitchenTimerState = RESET_STATE; }

// キッチンタイマー状態をカウント設定状態へ変更

void SetKitchenTimerStateToSetting(void) {
    KitchenTimerState = COUNTTIME_SETTING_STATE;
}

// キッチンタイマー状態をカウントダウン中へ変更

void SetKitchenTimerStateToGoing(void) {
    KitchenTimerState = COUNTDOWN_ONGOING_STATE;
}

// キッチンタイマー状態をカウントダウン終了へ変更

void SetKitchenTimerStateToEnd(void) {
    KitchenTimerState = COUNTDOWN_END_STATE;
}

/**
 End of File
 */
