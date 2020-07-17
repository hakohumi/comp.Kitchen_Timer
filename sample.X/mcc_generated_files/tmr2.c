// 10msタイマ

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

    // SW1が押されてチャタフラグが立ったら
    if (SW1.ChattaFlg == ON) {

        // SW1のポートの値を読み取る
        SW1.ReadValue = ~((bool)SW1_Value);

        // チャタ状態は？
        switch (SW1.ChattaState) {

            case RISING:
                // ------------------------------------------------------------
                // 立ち上がりチェック状態
                // ------------------------------------------------------------
                // SWのポート値はONか？
                if (SW1.ReadValue == ON) {
                    // チェックカウントを1増加
                    SW1.CheckCount++;

                    // チェックカウントは3以上か
                    if (SW1.CheckCount >= 3) {
                        // チャタ状態を継続状態へ
                        SW1.ChattaState = ONGOING;
                        // SW1の状態をONへ
                        SW1.State = ON;
                        // チェックカウントをクリア
                        SW1.CheckCount = 0;
                    }

                } else {
                    // SWのポート値がOFFの場合
                    // チェックカウントをクリア
                    SW1.CheckCount = 0;
                    // ------------------------------------------------------------
                    // 立ち下がり処理
                    // ------------------------------------------------------------
                    // SWカウントをクリア
                    //                        SW1.SWCount = 0;
                    // SW1のチャタフラグをクリア
                    SW1.ChattaFlg = 0;
                    // SW1のIOC割込みフラグをON
                    IOC_INT_SW1_ENABLE();
                    // ------------------------------------------------------------
                }
                break;

            case ONGOING:

                // SWカウントは250未満か
                if (SW1.SWCount < 250) {
                    // SWカウントを1増やす
                    SW1.SWCount++;
                }

                // SW1のポート値は？
                if (SW1.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    SW1.ChattaState = FALLING;
                    SW1.CheckCount++;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING:

                // SWカウントは250未満か
                if (SW1.SWCount < 250) {
                    // SWカウントを1増やす
                    SW1.SWCount++;
                }

                // SW1のポート値はOFFか
                if (SW1.ReadValue == OFF) {
                    SW1.CheckCount++;

                    // チェックカウントは3以上か
                    if (SW1.CheckCount >= 3) {
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        SW1.ChattaState = RISING;
                        // チェックカウントをクリア
                        SW1.CheckCount = 0;
                        // SW1の状態をOFF
                        SW1.State = OFF;

                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        SW1.SWCount = 0;
                        // SW1のチャタフラグをクリア
                        SW1.ChattaFlg = 0;
                        // SW1のIOC割込みフラグをON
                        IOC_INT_SW1_ENABLE();
                        // ------------------------------------------------------------
                    }
                } else {

                    // チャタ状態を継続状態へ
                    SW1.ChattaState = ONGOING;
                    // チェックカウントをクリア
                    SW1.CheckCount = 0;

                }
                break;
        }
    }

    // SW2が押されてチャタフラグが立ったら
    if (SW2.ChattaFlg == ON) {

        // SW2のポートの値を読み取る
        SW2.ReadValue = ~((bool)SW2_Value);

        // チャタ状態は？
        switch (SW2.ChattaState) {

            case RISING:
                // ------------------------------------------------------------
                // 立ち上がりチェック状態
                // ------------------------------------------------------------
                // SWのポート値はONか？
                if (SW2.ReadValue == ON) {
                    // チェックカウントを1増加
                    SW2.CheckCount++;

                    // チェックカウントは3以上か
                    if (SW2.CheckCount >= 3) {
                        // チャタ状態を継続状態へ
                        SW2.ChattaState = ONGOING;
                        // SW2の状態をONへ
                        SW2.State = ON;
                        // チェックカウントをクリア
                        SW2.CheckCount = 0;
                    }

                } else {
                    // SWのポート値がOFFの場合
                    // チェックカウントをクリア
                    SW2.CheckCount = 0;
                    // ------------------------------------------------------------
                    // 立ち下がり処理
                    // ------------------------------------------------------------
                    // SWカウントをクリア
                    //                        SW2.SWCount = 0;
                    // SW2のチャタフラグをクリア
                    SW2.ChattaFlg = 0;
                    // SW2のIOC割込みフラグをON
                    IOC_INT_SW2_ENABLE();
                    // ------------------------------------------------------------
                }
                break;

            case ONGOING:

                // SWカウントは250未満か
                if (SW2.SWCount < 250) {
                    // SWカウントを1増やす
                    SW2.SWCount++;
                }

                // SW2のポート値は？
                if (SW2.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    SW2.ChattaState = FALLING;
                    SW2.CheckCount++;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING:

                // SWカウントは250未満か
                if (SW2.SWCount < 250) {
                    // SWカウントを1増やす
                    SW2.SWCount++;
                }

                // SW2のポート値はOFFか
                if (SW2.ReadValue == OFF) {
                    SW2.CheckCount++;

                    // チェックカウントは3以上か
                    if (SW2.CheckCount >= 3) {
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        SW2.ChattaState = RISING;
                        // チェックカウントをクリア
                        SW2.CheckCount = 0;
                        // SW2の状態をOFF
                        SW2.State = OFF;

                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        SW2.SWCount = 0;
                        // SW2のチャタフラグをクリア
                        SW2.ChattaFlg = 0;
                        // SW2のIOC割込みフラグをON
                        IOC_INT_SW2_ENABLE();
                        // ------------------------------------------------------------
                    }
                } else {

                    // チャタ状態を継続状態へ
                    SW2.ChattaState = ONGOING;
                    // チェックカウントをクリア
                    SW2.CheckCount = 0;

                }
                break;
        }
    }

    // SW3が押されてチャタフラグが立ったら
    if (SW3.ChattaFlg == ON) {

        // SW3のポートの値を読み取る
        SW3.ReadValue = ~((bool)SW3_Value);

        // チャタ状態は？
        switch (SW3.ChattaState) {

            case RISING:
                // ------------------------------------------------------------
                // 立ち上がりチェック状態
                // ------------------------------------------------------------
                // SWのポート値はONか？
                if (SW3.ReadValue == ON) {
                    // チェックカウントを1増加
                    SW3.CheckCount++;

                    // チェックカウントは3以上か
                    if (SW3.CheckCount >= 3) {
                        // チャタ状態を継続状態へ
                        SW3.ChattaState = ONGOING;
                        // SW3の状態をONへ
                        SW3.State = ON;
                        // チェックカウントをクリア
                        SW3.CheckCount = 0;
                    }

                } else {
                    // SWのポート値がOFFの場合
                    // チェックカウントをクリア
                    SW3.CheckCount = 0;
                    // ------------------------------------------------------------
                    // 立ち下がり処理
                    // ------------------------------------------------------------
                    // SWカウントをクリア
                    //                        SW3.SWCount = 0;
                    // SW3のチャタフラグをクリア
                    SW3.ChattaFlg = 0;
                    // SW3のIOC割込みフラグをON
                    IOC_INT_SW3_ENABLE();
                    // ------------------------------------------------------------
                }
                break;

            case ONGOING:

                // SWカウントは250未満か
                if (SW3.SWCount < 250) {
                    // SWカウントを1増やす
                    SW3.SWCount++;
                }

                // SW3のポート値は？
                if (SW3.ReadValue == OFF) {
                    // チャタ状態を立ち下がりチャタチェック状態へ
                    SW3.ChattaState = FALLING;
                    SW3.CheckCount++;
                }
                break;

                // ------------------------------------------------------------
                // 立ち下がり時チャタチェック
                // ------------------------------------------------------------
            case FALLING:

                // SWカウントは250未満か
                if (SW3.SWCount < 250) {
                    // SWカウントを1増やす
                    SW3.SWCount++;
                }

                // SW3のポート値はOFFか
                if (SW3.ReadValue == OFF) {
                    SW3.CheckCount++;

                    // チェックカウントは3以上か
                    if (SW3.CheckCount >= 3) {
                        // チャタ状態を立ち上がり時チャタチェック状態へ
                        SW3.ChattaState = RISING;
                        // チェックカウントをクリア
                        SW3.CheckCount = 0;
                        // SW3の状態をOFF
                        SW3.State = OFF;

                        // ------------------------------------------------------------
                        // 立ち下がり処理
                        // ------------------------------------------------------------
                        // SWカウントをクリア
                        SW3.SWCount = 0;
                        // SW3のチャタフラグをクリア
                        SW3.ChattaFlg = 0;
                        // SW3のIOC割込みフラグをON
                        IOC_INT_SW3_ENABLE();
                        // ------------------------------------------------------------
                    }
                } else {

                    // チャタ状態を継続状態へ
                    SW3.ChattaState = ONGOING;
                    // チェックカウントをクリア
                    SW3.CheckCount = 0;
                }
                break;
        }
    }
}

/**
  End of File
 */