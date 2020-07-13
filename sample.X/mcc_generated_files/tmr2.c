/**
  TMR2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr2.c

  @Summary
    This is the generated driver implementation file for the TMR2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

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
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
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

#include <xc.h>
#include "tmr2.h"


/**
  Section: Global Variables Definitions
 */

void (*TMR2_InterruptHandler)(void);

extern bool Timer10msFlag;
extern SWState_t SW1;
extern SWState_t SW2;
extern SWState_t SW3;

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
    TMR2_SetInterruptHandler(TMR2_DefaultInterruptHandler);

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

void TMR2_LoadPeriodRegister(uint8_t periodVal) {
    PR2 = periodVal;
}

void TMR2_ISR(void) {

    // clear the TMR2 interrupt flag
    PIR1bits.TMR2IF = 0;

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    TMR2_CallBack();
}

void TMR2_CallBack(void) {
    // Add your custom callback code here
    // this code executes every TMR2_INTERRUPT_TICKER_FACTOR periods of TMR2
    if (TMR2_InterruptHandler) {
        TMR2_InterruptHandler();
    }
}

void TMR2_SetInterruptHandler(void (* InterruptHandler)(void)) {
    TMR2_InterruptHandler = InterruptHandler;
}

void TMR2_DefaultInterruptHandler(void) {

    // add your TMR2 interrupt custom code
    // or set custom function using TMR2_SetInterruptHandler()

    // SW1が押されて状態変化割込みでフラグが立ったら
    if (SW1.IntFlg == true) {

        // SW1のポートの値を読み取る
        SW1.ReadValue = ~(SW1_Value);

        // チャタ状態は？
        switch (SW1.ChattaState) {

            case RISING:
                // ------------------------------------------------------------
                // 立ち上がりチェック
                // ------------------------------------------------------------
                // チェックカウントは3以上か
                if (SW1.CheckCount >= 3) {
                    // 立ち上がり比較フラグの値は1か
                    if (SW1.RiseCompareFlg == 1) {
                        // チャタ状態を継続状態へ
                        SW1.ChattaState = ONGOING;
                        // SW1の状態をONへ
                        SW1.State = ON;
                        // チェックカウントをクリア
                        SW1.CheckCount = 0;
                    } else {
                        // チェックカウントをクリア
                        SW1.CheckCount = 0;
                        // 立ち上がり比較フラグを1へ初期化
                        SW1.RiseCompareFlg = 1;
                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        //                        SW1.SWCount = 0;
                        // SW1の状態をOFF
                        SW1.State = OFF;
                        // SW1の割り込みフラグをクリア
                        SW1.IntFlg = 0;
                        // ------------------------------------------------------------
                    }

                } else {
                    // SWのポートの値と立ち上がり比較フラグを論理積する
                    SW1.RiseCompareFlg &= SW1.ReadValue;

                    // チェックカウントを1増やす
                    SW1.CheckCount++;
                }
                break;

            case ONGOING:

                // SWカウントを1増やす
                SW1.SWCount++;
                // SW1のポート値は？
                if (SW1.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    SW1.ChattaState = FALLING;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING:

                // SWカウントを1増やす
                SW1.SWCount++;
                // チェックカウントは3以上か
                if (SW1.CheckCount >= 3) {
                    // 立ち下がり比較フラグの値は0か
                    if (SW1.FallCompareFlg == 0) {
                        // ------------------------------------------------------------
                        // 立ち下がり比較フラグの値は0
                        // ------------------------------------------------------------
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        SW1.ChattaState = RISING;
                        // チェックカウントをクリア
                        SW1.CheckCount = 0;
                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        SW1.SWCount = 0;
                        // SW1の状態をOFF
                        SW1.State = OFF;
                        // SW1の割り込みフラグをクリア
                        SW1.IntFlg = 0;
                        // ------------------------------------------------------------
                    } else {
                        // ------------------------------------------------------------
                        // 立ち下がり比較フラグの値は0ではない
                        // ------------------------------------------------------------
                        // チャタ状態を継続状態へ
                        SW1.ChattaState = ONGOING;
                        // チェックカウントをクリア
                        SW1.CheckCount = 0;
                        // 立ち下がり比較フラグを0へ初期化
                        SW1.FallCompareFlg = 0;
                    }
                } else {
                    // ------------------------------------------------------------
                    // チェックカウントは3未満
                    // ------------------------------------------------------------
                    // SWのポートの値と立ち下がり比較フラグを論理和する
                    SW1.FallCompareFlg |= SW1.ReadValue;

                    SW1.CheckCount++;
                }
                break;
        }
    }
    
    // SW2が押されて状態変化割込みでフラグが立ったら
    if (SW2.IntFlg == true) {

        // SW2のポートの値を読み取る
        SW2.ReadValue = ~(SW2_Value);

        // チャタ状態は？
        switch (SW2.ChattaState) {

            case RISING:
                // ------------------------------------------------------------
                // 立ち上がりチェック
                // ------------------------------------------------------------
                // チェックカウントは3以上か
                if (SW2.CheckCount >= 3) {
                    // 立ち上がり比較フラグの値は1か
                    if (SW2.RiseCompareFlg == 1) {
                        // チャタ状態を継続状態へ
                        SW2.ChattaState = ONGOING;
                        // SW2の状態をONへ
                        SW2.State = ON;
                        // チェックカウントをクリア
                        SW2.CheckCount = 0;
                    } else {
                        // チェックカウントをクリア
                        SW2.CheckCount = 0;
                        // 立ち上がり比較フラグを1へ初期化
                        SW2.RiseCompareFlg = 1;
                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        //                        SW2.SWCount = 0;
                        // SW2の状態をOFF
                        SW2.State = OFF;
                        // SW2の割り込みフラグをクリア
                        SW2.IntFlg = 0;
                        // ------------------------------------------------------------
                    }

                } else {
                    // SWのポートの値と立ち上がり比較フラグを論理積する
                    SW2.RiseCompareFlg &= SW2.ReadValue;

                    // チェックカウントを1増やす
                    SW2.CheckCount++;
                }
                break;

            case ONGOING:

                // SWカウントを1増やす
                SW2.SWCount++;
                // SW2のポート値は？
                if (SW2.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    SW2.ChattaState = FALLING;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING:

                // SWカウントを1増やす
                SW2.SWCount++;
                // チェックカウントは3以上か
                if (SW2.CheckCount >= 3) {
                    // 立ち下がり比較フラグの値は0か
                    if (SW2.FallCompareFlg == 0) {
                        // ------------------------------------------------------------
                        // 立ち下がり比較フラグの値は0
                        // ------------------------------------------------------------
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        SW2.ChattaState = RISING;
                        // チェックカウントをクリア
                        SW2.CheckCount = 0;
                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        SW2.SWCount = 0;
                        // SW2の状態をOFF
                        SW2.State = OFF;
                        // SW2の割り込みフラグをクリア
                        SW2.IntFlg = 0;
                        // ------------------------------------------------------------
                    } else {
                        // ------------------------------------------------------------
                        // 立ち下がり比較フラグの値は0ではない
                        // ------------------------------------------------------------
                        // チャタ状態を継続状態へ
                        SW2.ChattaState = ONGOING;
                        // チェックカウントをクリア
                        SW2.CheckCount = 0;
                        // 立ち下がり比較フラグを0へ初期化
                        SW2.FallCompareFlg = 0;
                    }
                } else {
                    // ------------------------------------------------------------
                    // チェックカウントは3未満
                    // ------------------------------------------------------------
                    // SWのポートの値と立ち下がり比較フラグを論理和する
                    SW2.FallCompareFlg |= SW2.ReadValue;

                    SW2.CheckCount++;
                }
                break;
        }
    }
    
    // SW3が押されて状態変化割込みでフラグが立ったら
    if (SW3.IntFlg == true) {

        // SW3のポートの値を読み取る
        SW3.ReadValue = ~(SW3_Value);

        // チャタ状態は？
        switch (SW3.ChattaState) {

            case RISING:
                // ------------------------------------------------------------
                // 立ち上がりチェック
                // ------------------------------------------------------------
                // チェックカウントは3以上か
                if (SW3.CheckCount >= 3) {
                    // 立ち上がり比較フラグの値は1か
                    if (SW3.RiseCompareFlg == 1) {
                        // チャタ状態を継続状態へ
                        SW3.ChattaState = ONGOING;
                        // SW3の状態をONへ
                        SW3.State = ON;
                        // チェックカウントをクリア
                        SW3.CheckCount = 0;
                    } else {
                        // チェックカウントをクリア
                        SW3.CheckCount = 0;
                        // 立ち上がり比較フラグを1へ初期化
                        SW3.RiseCompareFlg = 1;
                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        //                        SW3.SWCount = 0;
                        // SW3の状態をOFF
                        SW3.State = OFF;
                        // SW3の割り込みフラグをクリア
                        SW3.IntFlg = 0;
                        // ------------------------------------------------------------
                    }

                } else {
                    // SWのポートの値と立ち上がり比較フラグを論理積する
                    SW3.RiseCompareFlg &= SW3.ReadValue;

                    // チェックカウントを1増やす
                    SW3.CheckCount++;
                }
                break;

            case ONGOING:

                // SWカウントを1増やす
                SW3.SWCount++;
                // SW3のポート値は？
                if (SW3.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    SW3.ChattaState = FALLING;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING:

                // SWカウントを1増やす
                SW3.SWCount++;
                // チェックカウントは3以上か
                if (SW3.CheckCount >= 3) {
                    // 立ち下がり比較フラグの値は0か
                    if (SW3.FallCompareFlg == 0) {
                        // ------------------------------------------------------------
                        // 立ち下がり比較フラグの値は0
                        // ------------------------------------------------------------
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        SW3.ChattaState = RISING;
                        // チェックカウントをクリア
                        SW3.CheckCount = 0;
                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        SW3.SWCount = 0;
                        // SW3の状態をOFF
                        SW3.State = OFF;
                        // SW3の割り込みフラグをクリア
                        SW3.IntFlg = 0;
                        // ------------------------------------------------------------
                    } else {
                        // ------------------------------------------------------------
                        // 立ち下がり比較フラグの値は0ではない
                        // ------------------------------------------------------------
                        // チャタ状態を継続状態へ
                        SW3.ChattaState = ONGOING;
                        // チェックカウントをクリア
                        SW3.CheckCount = 0;
                        // 立ち下がり比較フラグを0へ初期化
                        SW3.FallCompareFlg = 0;
                    }
                } else {
                    // ------------------------------------------------------------
                    // チェックカウントは3未満
                    // ------------------------------------------------------------
                    // SWのポートの値と立ち下がり比較フラグを論理和する
                    SW3.FallCompareFlg |= SW3.ReadValue;

                    SW3.CheckCount++;
                }
                break;
        }
    }
}

/**
  End of File
 */