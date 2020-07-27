
#include "CountClass/reset.h"

#include "CountClass/CountClass.h"
#include "LCDClass.h"
#include "tmr1.h"

void reset(void) {
    // キッチンタイマー状態をカウントダウン設定へ変更
    SetKitchenTimerStateToSetting();

    // カウント時間を00m00sへ設定
    SetCountMinute(0);
    SetCountSecond(0);

    // UpdateLCDフラグをON
    SetUpdateLCDFlgON();

    // タイマレジスタのクリア
    TMR1_Reload();
}