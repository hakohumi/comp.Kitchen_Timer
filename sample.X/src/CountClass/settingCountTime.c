#include "CountClass/settingCountTime.h"

#include "CountClass/CountClass.h"
#include "InputClass.h"
#include "LCDClass.h"
#include "tmr1.h"

uint8_t detectSWState(SWState_t *i_SW);

// カウント時間設定

void settingCountTime() {
    // 分スイッチ処理
    MinuteCountTime = detectSWState(&MinuteSW);

    // 秒スイッチ処理
    SecondCountTime = detectSWState(&SecondSW);

    // スタートストップスイッチ状態はONか
    if (StartStopSW.PushState == ON_STATE) {
        // スタートストップスイッチ状態をOFFにする
        StartStopSW.PushState = OFF_STATE;
        // 0.5秒タイマ割込みの許可
        TMR500MS_TMRInterruptEnable();
        // キッチンタイマー状態をカウントダウン中へ変更
        SetKitchenTimerStateToGoing();
    }

    // リセットスイッチ状態はONか？
    if (IsPushedResetSW == ON_STATE) {
        // キッチンタイマー状態をリセットへ変更
        SetKitchenTimerStateToReset();
    }
}

uint8_t detectSWState(SWState_t *i_SW) {
    uint8_t l_retval = 0;

    // スイッチのタイミングフラグ
    if (i_SW->TimingFlag == ON) {
        // スイッチの状態は？
        switch (i_SW->PushState) {
                // 押されていない
            case OFF_STATE:
                // なにもしない
                break;

                // 短押しと長押し1段階目
            case SHORT_PUSH_STATE:
            case LONG_STG1_STATE:
                // 1分増やす
                l_retval = 1;

                // UpdateLCDFlg をON
                SetUpdateLCDFlgON();
                break;
                // 長押し2段階目
            case LONG_STG2_STATE:
                // 10分増やす
                l_retval = 10;

                // UpdateLCDFlg をON
                SetUpdateLCDFlgON();
                break;
        }
    }
    return l_retval;
}
