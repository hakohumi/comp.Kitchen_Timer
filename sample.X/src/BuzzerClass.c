#include "BuzzerClass.h"

#include "tmr4.h"

uint8_t BuzzerTimingCount = 0;
bool BuzzerTimingFlg = OFF;

void UpdateBuzzer(void) {
    // 今、鳴っているかどうかを判断する
    static bool BuzzerState = OFF;

    if (KitchenTimerState == COUNTDOWN_END_STATE) {
        if (BuzzerTimingFlg == ON) {
            if (BuzzerState == OFF) {
                TMR4_StartTimer();
                BuzzerState = ON;
            }
        } else {
            if (BuzzerState == ON) {
                TMR4_StopTimer();
                BuzzerState = OFF;
            }
        }
    } else {
        // キッチンタイマーの状態がその他の場合
        // 要らないときはならないようにする
        if (BuzzerState == ON) {
            TMR4_StopTimer();
            BuzzerState = OFF;
        }
    }
}