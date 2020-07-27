
#include "CountClass.h"

// カウント時間(分)
uint8_t MinuteCountTime = 0;
// カウント時間(秒)
uint8_t SecondCountTime = 0;

// カウントダウン終了カウント
uint8_t CountDownEndCount = 0;

// カウント時間設定
void SettingCountTime() {
    // 分スイッチ処理

    // 秒スイッチ処理

    // スタートストップスイッチ状態はONか
    if (StartStopSW.PushedState == ON) {
        // スタートストップスイッチ状態をOFFにする
        StartStopSW.PushedState = OFF;
        // 0.5秒タイマ割込みの許可
        TMR500MS_TMRInterruptEnable();
        // キッチンタイマー状態をカウントダウン中へ変更
        KitchenTimerState = COUNTDOWN_RUNNING_STATE;
    }

    // リセットスイッチ状態はONか？
    if (ResetSWState == ON_STATE) {
        KitchenTimerState = RESET_STATE;
    }
}

void detectSWState(SWState_t *i_SW) {
    // スイッチのタイミングフラグ
    if (i_SW->TimingFlag == ON) {
        // スイッチの状態は？
        switch (i_SW->PushState) {
            case OFF_STATE:
                break;
            case SHORT_PUSH_STATE:
                break;
            case LONG_STG1_STATE:
                break;
            case LONG_STG2_STATE:
                break;
        }
    }
