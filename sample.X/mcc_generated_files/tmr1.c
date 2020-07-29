// 500msタイマ

/**
  TMR1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated driver implementation file for the TMR1 driver using
  PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.3
        Device            :  PIC16F1827
        Driver Version    :  2.11
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

#include "tmr1.h"

#include <xc.h>

#include "CountClass.h"
#include "LCDClass.h"
#include "common.h"

/**
  Section: Global Variables Definitions
 */
volatile uint16_t timer1ReloadVal;
void (*TMR1_InterruptHandler)(void);

// 1秒フラグ
bool Is1sFlg = ON;

// カウントダウン終了カウント CountClass.c
extern uint8_t CountDownEndCount;

/**
  Section: TMR1 APIs
 */

void TMR1_Initialize(void) {
    // Set the Timer to the options selected in the GUI

    // T1GSS T1G_pin; TMR1GE disabled; T1GTM disabled; T1GPOL low; T1GGO done;
    // T1GSPM disabled;
    T1GCON = 0x00;

    // TMR1H 11;
    TMR1H = 0x0B;

    // TMR1L 220;
    TMR1L = 0xDC;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR1IF = 0;

    // Load the TMR value to reload variable
    timer1ReloadVal = (uint16_t)((TMR1H << 8) | TMR1L);

    // Enabling TMR1 interrupt.
    PIE1bits.TMR1IE = 1;

    // Set Default Interrupt Handler
    // TMR1_SetInterruptHandler(TMR1_DefaultInterruptHandler);

    // T1CKPS 1:1; T1OSCEN disabled; nT1SYNC synchronize; TMR1CS FOSC/4; TMR1ON
    // enabled;
    T1CON = 0x00;
}

void TMR1_StartTimer(void) {
    // Start the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 1;
}

void TMR1_StopTimer(void) {
    // Stop the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 0;
}

uint16_t TMR1_ReadTimer(void) {
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;

    readValLow = TMR1L;
    readValHigh = TMR1H;

    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR1_WriteTimer(uint16_t timerVal) {
    if (T1CONbits.nT1SYNC == 1) {
        // Stop the Timer by writing to TMRxON bit
        T1CONbits.TMR1ON = 0;

        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;

        // Start the Timer after writing to the register
        T1CONbits.TMR1ON = 1;
    } else {
        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;
    }
}

void TMR1_Reload(void) { TMR1_WriteTimer(timer1ReloadVal); }

void TMR1_StartSinglePulseAcquisition(void) { T1GCONbits.T1GGO = 1; }

uint8_t TMR1_CheckGateValueStatus(void) { return (T1GCONbits.T1GVAL); }

void TMR1_ISR(void) {
    // Clear the TMR1 interrupt flag
    PIR1bits.TMR1IF = 0;
    TMR1_WriteTimer(timer1ReloadVal);

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes

    // TMR1_CallBack();
    TMR1_DefaultInterruptHandler();
}

// void TMR1_CallBack(void) {
//     // Add your custom callback code here
//     if (TMR1_InterruptHandler) {
//         TMR1_InterruptHandler();
//     }
// }

// void TMR1_SetInterruptHandler(void (*InterruptHandler)(void)) {
//     TMR1_InterruptHandler = InterruptHandler;
// }

inline void TMR1_DefaultInterruptHandler(void) {
    // add your TMR1 interrupt custom code
    // or set custom function using TMR1_SetInterruptHandler()

    // 1秒フラグ
    if (Is1sFlg == ON) {
        
        if (KitchenTimerState == COUNTDOWN_ONGOING_STATE) {
            // カウントは00m00sか
            if (MinuteCountTime == 0 && SecondCountTime == 0) {
                // カウントダウン終了カウントを0へ初期化
                CountDownEndCount = 0;

                // キッチンタイマー状態をカウントダウン終了へ変更
                SetKitchenTimerStateToEnd();
            } else {
                // カウント時間を1秒減少させる
                CountDown();
            }
            //キッチンタイマー状態がカウントダウン終了か？
        } else if (KitchenTimerState == COUNTDOWN_END_STATE) {
            // カウントダウン終了カウントを1増加
            CountDownEndCount++;
            // キッチンタイマー状態はカウントダウン中か？
        }

        // 1秒フラグをOFF
        Is1sFlg = OFF;
    } else {
        // 1秒フラグがOFFの時

        // 1秒フラグをONへ
        Is1sFlg = ON;
    }

    // UpdateLCDフラグをON
    SetUpdateLCDFlgON();
}

/**
  End of File
 */
