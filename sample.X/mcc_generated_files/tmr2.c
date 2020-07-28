// 10msタイマ

/**
  TMR2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr2.c

  @Summary
    This is the generated driver implementation file for the TMR2 driver using
  PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR2.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.3
        Device            :  PIC16F1827
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB 	          :  MPLAB X 5.40
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

/**
  Section: Included Files
 */

#include "tmr2.h"

#include <xc.h>

#include "InputClass.h"
#include "common.h"

/**
  Section: Global Variables Definitions
 */

void (*TMR2_InterruptHandler)(void);

extern bool Timer10msFlag;

/**
  Section: TMR2 APIs
 */

void TMR2_Initialize(void) {
    // Set TMR2 to the options selected in the User Interface

    // PR2 249;
    PR2 = 0xF9;

    // TMR2 0;
    TMR2 = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR2IF = 0;

    // Enabling TMR2 interrupt.
    PIE1bits.TMR2IE = 1;

    // Set Default Interrupt Handler
    // TMR2_SetInterruptHandler(TMR2_DefaultInterruptHandler);

    // T2CKPS 1:1; T2OUTPS 1:5; TMR2ON on;
    T2CON = 0x24;
}

void TMR2_StartTimer(void) {
    // Start the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 1;
}

void TMR2_StopTimer(void) {
    // Stop the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 0;
}

uint8_t TMR2_ReadTimer(void) {
    uint8_t readVal;

    readVal = TMR2;

    return readVal;
}

void TMR2_WriteTimer(uint8_t timerVal) {
    // Write to the Timer2 register
    TMR2 = timerVal;
}

void TMR2_LoadPeriodRegister(uint8_t periodVal) { PR2 = periodVal; }

void TMR2_ISR(void) {
    // clear the TMR2 interrupt flag
    PIR1bits.TMR2IF = 0;

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes

    // TMR2_CallBack();
    TMR2_DefaultInterruptHandler();
}

// void TMR2_CallBack(void) {
//     // Add your custom callback code here
//     // this code executes every TMR2_INTERRUPT_TICKER_FACTOR periods of TMR2
//     if (TMR2_InterruptHandler) {
//         TMR2_InterruptHandler();
//     }
// }

// void TMR2_SetInterruptHandler(void (*InterruptHandler)(void)) {
//     TMR2_InterruptHandler = InterruptHandler;
// }

void TMR2_DefaultInterruptHandler(void) {
    // add your TMR2 interrupt custom code
    // or set custom function using TMR2_SetInterruptHandler()

    // MinuteSWが押されてチャタフラグが立ったら
    if (MinuteSW.ChattaFlg == ON) {
        // MinuteSWのポートの値を読み取る
        MinuteSW.ReadValue = ~((bool)SW1_Value);

        // チャタ状態は？
        switch (MinuteSW.ChattaState) {
            case RISING_STATE:
                // ------------------------------------------------------------
                // 立ち上がりチェック状態
                // ------------------------------------------------------------
                // SWのポート値はONか？
                if (MinuteSW.ReadValue == ON) {
                    // チェックカウントを1増加
                    MinuteSW.CheckCount++;

                    // チェックカウントは3以上か
                    if (MinuteSW.CheckCount >= 3) {
                        // チャタ状態を継続状態へ
                        MinuteSW.ChattaState = ONGOING_STATE;
                        // MinuteSWの状態をONへ
                        MinuteSW.PushState = ON;
                        // チェックカウントをクリア
                        MinuteSW.CheckCount = 0;
                    }

                } else {
                    // SWのポート値がOFFの場合
                    // チェックカウントをクリア
                    MinuteSW.CheckCount = 0;
                    // ------------------------------------------------------------
                    // 立ち下がり処理
                    // ------------------------------------------------------------
                    // SWカウントをクリア
                    //                        MinuteSW.SWCount = 0;
                    // MinuteSWのチャタフラグをクリア
                    MinuteSW.ChattaFlg = 0;
                    // MinuteSWのIOC割込みフラグをON
                    SW1_IOCInterruptEnable();
                    // ------------------------------------------------------------
                }
                break;

            case ONGOING_STATE:

                // SWカウントは250未満か
                if (MinuteSW.SWCount < 250) {
                    // SWカウントを1増やす
                    MinuteSW.SWCount++;
                }

                // MinuteSWのポート値は？
                if (MinuteSW.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    MinuteSW.ChattaState = FALLING_STATE;
                    MinuteSW.CheckCount++;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING_STATE:

                // SWカウントは250未満か
                if (MinuteSW.SWCount < 250) {
                    // SWカウントを1増やす
                    MinuteSW.SWCount++;
                }

                // MinuteSWのポート値はOFFか
                if (MinuteSW.ReadValue == OFF) {
                    MinuteSW.CheckCount++;

                    // チェックカウントは3以上か
                    if (MinuteSW.CheckCount >= 3) {
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        MinuteSW.ChattaState = RISING_STATE;
                        // チェックカウントをクリア
                        MinuteSW.CheckCount = 0;
                        // MinuteSWの状態をOFF
                        MinuteSW.PushState = OFF;

                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        MinuteSW.SWCount = 0;
                        // MinuteSWのチャタフラグをクリア
                        MinuteSW.ChattaFlg = 0;
                        // MinuteSWのIOC割込みフラグをON
                        SW1_IOCInterruptEnable();
                        // ------------------------------------------------------------
                    }
                } else {
                    // チャタ状態を継続状態へ
                    MinuteSW.ChattaState = ONGOING_STATE;
                    // チェックカウントをクリア
                    MinuteSW.CheckCount = 0;
                }
                break;
        }
    }

    // SW2が押されてチャタフラグが立ったら
    if (SecondSW.ChattaFlg == ON) {
        // SW2のポートの値を読み取る
        SecondSW.ReadValue = ~((bool)SW2_Value);

        // チャタ状態は？
        switch (SecondSW.ChattaState) {
            case RISING_STATE:
                // ------------------------------------------------------------
                // 立ち上がりチェック状態
                // ------------------------------------------------------------
                // SWのポート値はONか？
                if (SecondSW.ReadValue == ON) {
                    // チェックカウントを1増加
                    SecondSW.CheckCount++;

                    // チェックカウントは3以上か
                    if (SecondSW.CheckCount >= 3) {
                        // チャタ状態を継続状態へ
                        SecondSW.ChattaState = ONGOING_STATE;
                        // SW2の状態をONへ
                        SecondSW.PushState = ON;
                        // チェックカウントをクリア
                        SecondSW.CheckCount = 0;
                    }

                } else {
                    // SWのポート値がOFFの場合
                    // チェックカウントをクリア
                    SecondSW.CheckCount = 0;
                    // ------------------------------------------------------------
                    // 立ち下がり処理
                    // ------------------------------------------------------------
                    // SWカウントをクリア
                    //                        SecondSW.SWCount = 0;
                    // SW2のチャタフラグをクリア
                    SecondSW.ChattaFlg = 0;
                    // SW2のIOC割込みフラグをON
                    SW2_IOCInterruptEnable();
                    // ------------------------------------------------------------
                }
                break;

            case ONGOING_STATE:

                // SWカウントは250未満か
                if (SecondSW.SWCount < 250) {
                    // SWカウントを1増やす
                    SecondSW.SWCount++;
                }

                // SW2のポート値は？
                if (SecondSW.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    SecondSW.ChattaState = FALLING_STATE;
                    SecondSW.CheckCount++;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING_STATE:

                // SWカウントは250未満か
                if (SecondSW.SWCount < 250) {
                    // SWカウントを1増やす
                    SecondSW.SWCount++;
                }

                // SW2のポート値はOFFか
                if (SecondSW.ReadValue == OFF) {
                    SecondSW.CheckCount++;

                    // チェックカウントは3以上か
                    if (SecondSW.CheckCount >= 3) {
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        SecondSW.ChattaState = RISING_STATE;
                        // チェックカウントをクリア
                        SecondSW.CheckCount = 0;
                        // SW2の状態をOFF
                        SecondSW.PushState = OFF;

                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        SecondSW.SWCount = 0;
                        // SW2のチャタフラグをクリア
                        SecondSW.ChattaFlg = 0;
                        // SW2のIOC割込みフラグをON
                        SW2_IOCInterruptEnable();
                        // ------------------------------------------------------------
                    }
                } else {
                    // チャタ状態を継続状態へ
                    SecondSW.ChattaState = ONGOING_STATE;
                    // チェックカウントをクリア
                    SecondSW.CheckCount = 0;
                }
                break;
        }
    }

    // SW3が押されてチャタフラグが立ったら
    if (StartStopSW.ChattaFlg == ON) {
        // SW3のポートの値を読み取る
        StartStopSW.ReadValue = ~((bool)SW3_Value);

        // チャタ状態は？
        switch (StartStopSW.ChattaState) {
            case RISING_STATE:
                // ------------------------------------------------------------
                // 立ち上がりチェック状態
                // ------------------------------------------------------------
                // SWのポート値はONか？
                if (StartStopSW.ReadValue == ON) {
                    // チェックカウントを1増加
                    StartStopSW.CheckCount++;

                    // チェックカウントは3以上か
                    if (StartStopSW.CheckCount >= 3) {
                        // チャタ状態を継続状態へ
                        StartStopSW.ChattaState = ONGOING_STATE;
                        // SW3の状態をONへ
                        StartStopSW.PushState = ON;
                        // チェックカウントをクリア
                        StartStopSW.CheckCount = 0;
                    }

                } else {
                    // SWのポート値がOFFの場合
                    // チェックカウントをクリア
                    StartStopSW.CheckCount = 0;
                    // ------------------------------------------------------------
                    // 立ち下がり処理
                    // ------------------------------------------------------------
                    // SWカウントをクリア
                    //                        StartStopSW.SWCount = 0;
                    // SW3のチャタフラグをクリア
                    StartStopSW.ChattaFlg = 0;
                    // SW3のIOC割込みフラグをON
                    SW3_IOCInterruptEnable();
                    // ------------------------------------------------------------
                }
                break;

            case ONGOING_STATE:

                // SWカウントは250未満か
                if (StartStopSW.SWCount < 250) {
                    // SWカウントを1増やす
                    StartStopSW.SWCount++;
                }

                // SW3のポート値は？
                if (StartStopSW.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    StartStopSW.ChattaState = FALLING_STATE;
                    StartStopSW.CheckCount++;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING_STATE:

                // SWカウントは250未満か
                if (StartStopSW.SWCount < 250) {
                    // SWカウントを1増やす
                    StartStopSW.SWCount++;
                }

                // SW3のポート値はOFFか
                if (StartStopSW.ReadValue == OFF) {
                    StartStopSW.CheckCount++;

                    // チェックカウントは3以上か
                    if (StartStopSW.CheckCount >= 3) {
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        StartStopSW.ChattaState = RISING_STATE;
                        // チェックカウントをクリア
                        StartStopSW.CheckCount = 0;
                        // SW3の状態をOFF
                        StartStopSW.PushState = OFF;

                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        StartStopSW.SWCount = 0;
                        // SW3のチャタフラグをクリア
                        StartStopSW.ChattaFlg = 0;
                        // SW3のIOC割込みフラグをON
                        SW3_IOCInterruptEnable();
                        // ------------------------------------------------------------
                    }
                } else {
                    // チャタ状態を継続状態へ
                    StartStopSW.ChattaState = ONGOING_STATE;
                    // チェックカウントをクリア
                    StartStopSW.CheckCount = 0;
                }
                break;
        }
    }

    /* -------------------------------------------------- */
    /* 分SWカウントタイミング処理                         */
    /* -------------------------------------------------- */

    // 分スイッチタイミングフラグがOFFか
    if (!MinuteSW.TimingFlag) {
        // 分スイッチタイミングカウントは25以上か？
        if (MinuteSW.TimingCount >= 25) {
            // 分スイッチタイミングフラグをONに
            MinuteSW.TimingFlag = ON;

            // 分スイッチタイミングカウントを0へ初期化
            MinuteSW.TimingCount = 0;
        } else {
            // 分スイッチタイミングカウントを1増加
            MinuteSW.TimingCount++;
        }
    }

    /* -------------------------------------------------- */
    /* 秒SWカウントタイミング処理                         */
    /* -------------------------------------------------- */

    // 秒スイッチタイミングフラグがOFFか
    if (!SecondSW.TimingFlag) {
        // 秒スイッチタイミングカウントは25以上か？
        if (SecondSW.TimingCount >= 25) {
            // 秒スイッチタイミングフラグをONに
            SecondSW.TimingFlag = ON;

            // 秒スイッチタイミングカウントを0へ初期化
            SecondSW.TimingCount = 0;
        } else {
            // 秒スイッチタイミングカウントを1増加
            SecondSW.TimingCount++;
        }
    }
}

/**
  End of File
 */