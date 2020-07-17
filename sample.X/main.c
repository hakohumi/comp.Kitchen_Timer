/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.3
        Device            :  PIC16F1827
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "tmr1.h"

/*
                         Main application
 */

bool Timer500msFlag;
bool Timer10msFlag;

SWState_t SW1 = {
    OFF, // ChattaFlg
    OFF, // ReadValue
    RISING, // ChattaState
    0, // CheckCount
    OFF, // State
    0, // SWCount
};

SWState_t SW2 = {
    OFF, // ChattaFlg
    OFF, // ReadValue
    RISING, // ChattaState
    0, // CheckCount
    OFF, // State
    0, // SWCount
};
SWState_t SW3 = {
    OFF, // ChattaFlg
    OFF, // ReadValue
    RISING, // ChattaState
    0, // CheckCount
    OFF, // State
    0, // SWCount
};

// キッチンタイマー状態 初期値:リセット状態
KITCHEN_TIMER_STATE_E KitchenTimerState = RESET_STATE;

void InputProcess(void);
void OutputProcess(void);

void main(void) {

    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();



    while (1) {
        InputProcess();
        OutputProcess();

    }
    // Add your application code

}

/**
 End of File
 */

// デバッグ用
// SWの状態で、LEDを点灯させる
// LED3 は SW1 と SW2 の 同時押し

void InputProcess() {

    //    if (SW1.State == 1 && SW2.State == 1) {
    //        LED3 = LED_ON;
    //    } else {
    //        LED3 = LED_OFF;

    if (SW1.State == 1) {
        //LED1_Toggle();
        LED1 = LED_ON;
    } else {
        LED1 = LED_OFF;
    }

    if (SW2.State == 1) {
        LED2 = LED_ON;
    } else {
        LED2 = LED_OFF;
    }
}

//}

void OutputProcess() {
    if (Is1sFlg == ON) {
        LED4 = LED_ON;
    } else {
        LED4 = LED_OFF;
    }
}