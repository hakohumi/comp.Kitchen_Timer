/*
 * File:   main.c
 * Author: fuminori.hakoishi
 *
 * Created on 2020/06/23, 10:06
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "SW.h"

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, MCLRE = ON
#pragma config CP = OFF, CPD = OFF, BOREN = ON, LVP = OFF

#define LED1 LATB1
#define LED2 LATB6

void initIntrrupt(void);
void initOsc(void);
void initPort(void);
void initIntTMR0(void);
void initIntTMR1(void);
void initIntCCP1(void);
void initIntExternal(void);

enum {
    SET_COUNTTIME_STATE,
    RUNNING_COUNTDOWN_STATE,
    STOP_COUNTDOWN_STATE,
    FINISH_COUNTDOWN_STATE
};

void main(void) {

    // 状態遷移用フラグ
    uint8_t State;

    // 初期状態 = カウント時間設定
    State = SET_COUNTTIME_STATE;

    // 割込み初期化
    initIntrrupt();

    while (true) {
        // 入力 更新
        UpdateSWState();

        switch (State) {
            case SET_COUNTTIME_STATE:
                // もし、分ボタンが押されていたら、
                // LEDをつける
                if (IsPushedMinuteSW()){
                    
                }
                break;
            case RUNNING_COUNTDOWN_STATE:
                break;
            case STOP_COUNTDOWN_STATE:
                break;
            case FINISH_COUNTDOWN_STATE:
                break;
        }
    }
    return;
}

void initIntrrupt(void) {

    initOsc();
    initPort();
    //    initIntTMR0();
    initIntTMR1();
    initIntCCP1();
    initIntExternal();

    // 周辺機器割り込みの許可
    INTCONbits.PEIE = 1;

    // 全体の割り込み許可
    INTCONbits.GIE = 1;

}

void initOsc(void) {
    // 内部クロックソースの設定 00 = FOSCの設定に合わせる
    OSCCONbits.SCS = 0x00;

}

void initPort(void) {
    // 電圧レベルの初期設定
    PORTB = 0xFF;
    PORTA = 0xFF;

    // 入出力設定 0 = OUT, 1 = INPUT
    // 使わないポートは、出力

    // PORTB RB0:外部割込み


    TRISB = 0x01; // RB0 入力
    TRISA = 0x21; // RA1 を入力、RA5は入力専用

    // 入力ポート、アナログ・デジタル設定
    ANSELB = 0x00;
    ANSELA = 0x00;

    // CCP1 PIN をRB3へ 選択
    // 1 = RB0, 0 = RB3
    APFCON0bits.CCP1SEL = 0;

}

void initIntTMR0(void) {

    // TIMER0のクロックソースを内部クロック
    OPTION_REGbits.TMR0CS = 0;

    // TIMER0のプリスケーラの割り当て 有効
    OPTION_REGbits.PSA = 0;
    // プリスケーラ設定 1:256
    OPTION_REGbits.PS = 0b111;

    // TMR0 レジスタのクリア
    TMR0 = 0x00;

    // Timer0 割り込みフラグ クリア
    INTCONbits.TMR0IF = 0;

    // Timer0 オーバーフロー割込み 許可
    INTCONbits.TMR0IE = 0;

}

// 16bit タイマ
// CCP1 比較モード用

void initIntTMR1(void) {

    /*
     * TMR1CS<1:0>: Timer1 Clock Source Select bits
11 = Timer1 clock source is Capacitive Sensing Oscillator (CAPOSC)
10 = Timer1 clock source is pin or oscillator:
If T1OSCEN = 0:
External clock from T1CKI pin (on the rising edge)
If T1OSCEN = 1:
Crystal oscillator on T1OSI/T1OSO pins
01 = Timer1 clock source is system clock (FOSC)
00 = Timer1 clock source is instruction clock (FOSC/4)
     */

    // 5. クロック源を内部クロックに設定
    T1CONbits.TMR1CS = 0b01;

    /*
     11 = 1:8 Prescale value
10 = 1:4 Prescale value
01 = 1:2 Prescale value
00 = 1:1 Prescale value
     */
    // 6. プリスケーラ設定
    T1CONbits.T1CKPS = 0b11;

    // 7. TMR1 レジスタのクリア
    TMR1H = 0x00;
    TMR1L = 0x00;

    // 8. タイマ1 起動
    T1CONbits.TMR1ON = 1;

}

/*
 * bit 3-0 CCPxM<3:0>: ECCPx Mode Select bits
0000 = Capture/Compare/PWM off (resets ECCPx module)
0001 = Reserved
0010 = Compare mode: toggle output on match
0011 = Reserved
0100 = Capture mode: every falling edge
0101 = Capture mode: every rising edge
0110 = Capture mode: every 4th rising edge
0111 = Capture mode: every 16th rising edge
1000 = Compare mode: initialize ECCPx pin low; set output on compare match (set CCPxIF)
1001 = Compare mode: initialize ECCPx pin high; clear output on compare match (set CCPxIF)
1010 = Compare mode: generate software interrupt only; ECCPx pin reverts to I/O state
1011 = Compare mode: Special Event Trigger (ECCPx resets T
 * */

void initIntCCP1(void) {
    // 1. CCPのモード設定
    // コンペアモード Special Event Trigger
    // Special Event Triggerを使用する場合、CCP4(RA4)Pinを使用
    CCP1CONbits.CCP1M = 0b1011;

    // 2. CCPR1レジスタ(16bit)に比較する値を代入
    CCPR1H = 0xF4;
    CCPR1L = 0x24;

    // 3. CCP1割込みフラグをクリア
    PIR1bits.CCP1IF = 0;

    // 4. CCCP1 割込み 許可
    PIE1bits.CCP1IE = 1;

}

void initIntExternal(void) {

    // 1. RB0を入力端子に設定

    // 2. 入力エッジを立ち下がりエッジに設定
    OPTION_REGbits.INTEDG = 0;

    // 3. 外部割込みフラグをクリア
    INTCONbits.INTF = 0;
    // 4. 外部割込みの許可
    INTCONbits.INTE = 1;

}

void __interrupt() ISR(void) {

    // TMR0 割り込み
    if (INTCONbits.TMR0IF == 1) {
        LED1 ^= 1;
        INTCONbits.TMR0IF = 0;
    }

    // CCP1 割込み
    if (PIR1bits.CCP1IF == 1) {
        LED1 ^= 1;
        PIR1bits.CCP1IF = 0;
    }

    // 外部割込みルーチン
    if (INTCONbits.INTF == 1) {
        LED2 ^= 1;
        // 外部割込みフラグをクリア
        INTCONbits.INTF = 0;
    }

}