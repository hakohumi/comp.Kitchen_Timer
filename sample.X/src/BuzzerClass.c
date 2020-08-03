#include "BuzzerClass.h"

#include "tmr4.h"

uint8_t BuzzerTimingCount = 0;
// 0.5sタイマで操作
bool BuzzerTimingFlg = OFF;
// 今、鳴っているかどうかを判断する
static bool buzzerState = OFF;
// 1秒間に鳴らした回数を保持する変数
static uint8_t buzzerCount = 0;

void UpdateBuzzer(void) {
    // カウントダウン終了状態の時
    if (KitchenTimerState == COUNTDOWN_END_STATE) {
        // 100ms経った時
        if (BuzzerTimingFlg == ON) {
            if (buzzerCount < 4) {
                // もし、ブザーがなっていなかったら、
                if (buzzerState == OFF) {
                    TMR4_StartTimer();
                    buzzerState = ON;
                    // ブザーがなっていたら、
                } else if (buzzerState == ON) {
                    TMR4_StopTimer();
                    buzzerState = OFF;
                    buzzerCount++;
                }

            } else {
                // 5回目の時

                // 鳴らさない

                // ブザーカウントの初期化
                buzzerCount = 0;
            }
            BuzzerTimingFlg = OFF;
        }
    } else {
        // キッチンタイマーの状態がその他の場合
        // 要らないときはならないようにする
        if (buzzerState == ON) {
            TMR4_StopTimer();
            buzzerState = OFF;
        }
    }
}

void ClrBuzzerCunt(void) {
    buzzerCount = 0;
}