/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.3
        Device            :  PIC16F1827
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB             :  MPLAB X 5.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include "pin_manager.h"
#include <stdbool.h>
#include "common.h"
#include "InputClass.h"





void (*IOCBF0_InterruptHandler)(void);
void (*IOCBF2_InterruptHandler)(void);
void (*IOCBF5_InterruptHandler)(void);

void PIN_MANAGER_Initialize(void) {
    /**
    LATx registers
     */
    LATA = 0xDF;
    LATB = 0xFF;

    /**
    TRISx registers
     */
    TRISA = 0x20;
    TRISB = 0x37;

    /**
    ANSELx registers
     */
    ANSELB = 0x00;
    ANSELA = 0x00;

    /**
    WPUx registers
     */
    WPUB = 0x00;
    WPUA = 0x00;
    OPTION_REGbits.nWPUEN = 1;


    /**
    APFCONx registers
     */
    APFCON0 = 0x00;
    APFCON1 = 0x00;

    /**
    IOCx registers 
     */
    //interrupt on change for group IOCBF - flag
    IOCBFbits.IOCBF0 = 0;
    //interrupt on change for group IOCBF - flag
    IOCBFbits.IOCBF2 = 0;
    //interrupt on change for group IOCBF - flag
    IOCBFbits.IOCBF5 = 0;
    //interrupt on change for group IOCBN - negative
    IOCBNbits.IOCBN0 = 1;
    //interrupt on change for group IOCBN - negative
    IOCBNbits.IOCBN2 = 1;
    //interrupt on change for group IOCBN - negative
    IOCBNbits.IOCBN5 = 1;
    //interrupt on change for group IOCBP - positive
    IOCBPbits.IOCBP0 = 0;
    //interrupt on change for group IOCBP - positive
    IOCBPbits.IOCBP2 = 0;
    //interrupt on change for group IOCBP - positive
    IOCBPbits.IOCBP5 = 0;



    // register default IOC callback functions at runtime; use these methods to register a custom function
    IOCBF0_SetInterruptHandler(IOCBF0_DefaultInterruptHandler);
    IOCBF2_SetInterruptHandler(IOCBF2_DefaultInterruptHandler);
    IOCBF5_SetInterruptHandler(IOCBF5_DefaultInterruptHandler);

    // Enable IOCI interrupt 
    INTCONbits.IOCIE = 1;

}

void PIN_MANAGER_IOC(void) {
    // interrupt on change for pin IOCBF0
    if (IOCBFbits.IOCBF0 == 1) {
        IOCBF0_ISR();
    }
    // interrupt on change for pin IOCBF2
    if (IOCBFbits.IOCBF2 == 1) {
        IOCBF2_ISR();
    }
    // interrupt on change for pin IOCBF5
    if (IOCBFbits.IOCBF5 == 1) {
        IOCBF5_ISR();
    }
}

/**
   IOCBF0 Interrupt Service Routine
 */
void IOCBF0_ISR(void) {

    // Add custom IOCBF0 code

    // Call the interrupt handler for the callback registered at runtime
    if (IOCBF0_InterruptHandler) {
        IOCBF0_InterruptHandler();
    }
    IOCBFbits.IOCBF0 = 0;
}

/**
  Allows selecting an interrupt handler for IOCBF0 at application runtime
 */
void IOCBF0_SetInterruptHandler(void (* InterruptHandler)(void)) {
    IOCBF0_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCBF0
 */
void IOCBF0_DefaultInterruptHandler(void) {
    // add your IOCBF0 interrupt custom code
    // or set custom function using IOCBF0_SetInterruptHandler()

    // SW3の処理 立ち下がり
    // スイッチフラグを立てる
    StartStopSW.ChattaFlg = true;

    // SW3の割り込み禁止
    SW3_IOCInterruptDisable();

}

/**
   IOCBF2 Interrupt Service Routine
 */
void IOCBF2_ISR(void) {

    // Add custom IOCBF2 code

    // Call the interrupt handler for the callback registered at runtime
    if (IOCBF2_InterruptHandler) {
        IOCBF2_InterruptHandler();
    }
    IOCBFbits.IOCBF2 = 0;
}

/**
  Allows selecting an interrupt handler for IOCBF2 at application runtime
 */
void IOCBF2_SetInterruptHandler(void (* InterruptHandler)(void)) {
    IOCBF2_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCBF2
 */
void IOCBF2_DefaultInterruptHandler(void) {
    // add your IOCBF2 interrupt custom code
    // or set custom function using IOCBF2_SetInterruptHandler()

    // MinuteSWの処理 立ち下がり
    // スイッチフラグを立てる
    MinuteSW.ChattaFlg = true;

    // MinuteSWの割り込み禁止
    SW1_IOCInterruptDisable();
}

/**
   IOCBF5 Interrupt Service Routine
 */
void IOCBF5_ISR(void) {

    // Add custom IOCBF5 code

    // Call the interrupt handler for the callback registered at runtime
    if (IOCBF5_InterruptHandler) {
        IOCBF5_InterruptHandler();
    }
    IOCBFbits.IOCBF5 = 0;
}

/**
  Allows selecting an interrupt handler for IOCBF5 at application runtime
 */
void IOCBF5_SetInterruptHandler(void (* InterruptHandler)(void)) {
    IOCBF5_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCBF5
 */
void IOCBF5_DefaultInterruptHandler(void) {
    // add your IOCBF5 interrupt custom code
    // or set custom function using IOCBF5_SetInterruptHandler()

    // SW2の処理 立ち下がり
    // スイッチフラグを立てる
    SecondSW.ChattaFlg = true;

    // SW2の割り込み禁止
    SW2_IOCInterruptDisable();

}

/**
 End of File
 */