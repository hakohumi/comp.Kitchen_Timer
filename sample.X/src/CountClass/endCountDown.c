#include "CountClass/endCountDown.h"

#include "CountClass/CountClass.h"
#include "InputClass.h"

void endCountDown(void) {
    // カウントダウン終了カウントが10以上か
    if (CountDownEndCount >= 10 || StartStopSW.PushState == ON_STATE) {
        // キッチンタイマー状態をリセット処理へ変更
        SetKitchenTimerStateToReset();
    }
}