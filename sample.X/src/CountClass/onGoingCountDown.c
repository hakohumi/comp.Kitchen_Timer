#include "CountClass/onGoingCountDown.h"

#include "CountClass/CountClass.h"
#include "InputClass.h"
#include "tmr1.h"

void onGoingCountDown(void) {
    // カウントは00m00sか
    if (MinuteCountTime == 0 && SecondCountTime == 0) {
        // 0.5秒タイマ割込みを禁止
        TMR500MS_TMRInterruptDisable();

        // カウントダウン終了カウントを0へ初期化
        CountDownEndCount = 0;

        // キッチンタイマー状態をカウントダウン終了へ変更
        SetKitchenTimerStateToEnd();
    }

    // スタートストップスイッチ状態はONか
    if (StartStopSW.PushState == ON_STATE) {
        // 0.5秒タイマ割込みを禁止
        TMR500MS_TMRInterruptDisable();
        // スタートストップスイッチ状態をOFFにする
        StartStopSW.PushState = OFF_STATE;
        // キッチンタイマー状態をカウント時間設定へ変更
        SetKitchenTimerStateToSetting();
    }

    // リセットスイッチ状態はONか？
    if (IsPushedResetSW == ON_STATE) {
        // 0.5秒タイマ割込み禁止
        TMR500MS_TMRInterruptDisable();
        // キッチンタイマー状態をリセット処理へ変更
        SetKitchenTimerStateToReset();
    }
}