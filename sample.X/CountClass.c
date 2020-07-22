
#include "CountClass.h"

// カウント時間(分)
uint8_t MinuteCountTime = 0;
// カウント時間(秒)
uint8_t SecondCountTime = 0;

// カウントダウン終了カウント
uint8_t CountDownEndCount = 0;

/*
状態遷移処理
CoutClass
*/

void StateTransferProcess(void) {
    switch (KitchenTimerState) {
        case COUNTTIME_SETTING_STATE:
            break;
        case COUNTDOWN_RUNNING_STATE:
            break;
        case COUNTDOWN_END_STATE:
            break;
        case RESET_STATE:
            break;
    }
}



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
    if (ResetSWState == ON) {
        KitchenTimerState = RESET_STATE;
    }
}

void detectSWState(SWState_t *i_SW){
    // スイッチのタイミングフラグ
    if(i_SW->TimingFlag == ON){
        // スイッチの状態は？
        if(i_SW->PushState != ON){
            
        }else{
            if(i_SW->LongPushState == )
        }
    }
}