#include "CountClass.h"

#include "InputClass.h"
#include "LCDClass.h"
#include "tmr1.h"

// 分の最大値
#define MINUTE_MAX 99
// 分の最低値
#define MINUTE_MIN 0

// 秒の最大値
#define SECOND_MAX 59
// 秒の最小値
#define SECOND_MIN 0

// カウント時間(分)
static uint8_t MinuteCountTime = 0;
// カウント時間(秒)
static uint8_t SecondCountTime = 0;
// カウントダウン終了カウント
uint8_t CountDownEndCount = 0;

static void settingCountTime(void);

static uint8_t detectSWState(SWState_t *i_SW);
static void onGoingCountDown(void);
static void endCountDown(void);
static void reset(void);

static uint8_t detectSWState(SWState_t *i_SW);

/*
状態遷移処理
CoutClass
 */

void StateTransferProcess(void) {
    switch (KitchenTimerState) {
        case COUNTTIME_SETTING_STATE:
            settingCountTime();
            break;
        case COUNTDOWN_RUNNING_STATE:
            onGoingCountDown();
            break;
        case COUNTDOWN_END_STATE:
            endCountDown();
            break;
        case RESET_STATE:
            reset();
            break;
    }
}

// 分カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 99 以外であれば、エラーとして false を返す

bool SetMinuteCount(uint8_t i_minute) {
    if ((i_minute < (uint8_t)MINUTE_MIN) || ((uint8_t)MINUTE_MAX < i_minute)) {
        return false;
    }

    MinuteCountTime = i_minute;

    return true;
}

// 秒カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 59 以外であれば、エラーとして false を返す

bool SetSecondCount(uint8_t i_second) {
    if ((i_second < (uint8_t)SECOND_MIN) || ((uint8_t)SECOND_MAX < i_second)) {
        return false;
    }

    SecondCountTime = i_second;

    return true;
}

// 引数で指定された値を分カウントに加算する
void AddMinuteCount(uint8_t i_minute) {
    // もし、入力された値が最大値より大きい場合
    // 最大値を設定
    // MINUTE_MAX = 99
    if (i_minute > MINUTE_MAX) {
        MinuteCountTime = MINUTE_MAX;
    } else {
        // 入力が 99 以下の場合

        // もし、加算後、分カウントが最大値より高い場合
        // 最大値より増えないようにする

        MinuteCountTime += i_minute;

        if (MinuteCountTime > MINUTE_MAX) {
            MinuteCountTime = MINUTE_MAX;
        }
    }
}

// 引数で指定された値を秒カウントに加算する
void AddSecondCount(uint8_t i_second) {
    // もし、加算後、秒カウントが最大値より高い場合
    // 最大値より下回るまで、分カウントに繰り上げる

    // 加算
    SecondCountTime += i_second;

    // 60で割った数分、繰り上げる
    AddMinuteCount(SecondCountTime / (SECOND_MAX + 1));
    // 60で割ったあまりを秒カウントへ格納する
    SecondCountTime %= (SECOND_MAX + 1);
}

// カウント時間設定

static void settingCountTime(void) {
    // 分スイッチ処理
    AddMinuteCount(detectSWState(&MinuteSW));
    // 秒スイッチ処理
    AddSecondCount(detectSWState(&SecondSW));

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

static uint8_t detectSWState(SWState_t *i_SW) {
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

static void onGoingCountDown(void) {
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

static void endCountDown(void) {
    // カウントダウン終了カウントが10以上か
    if (CountDownEndCount >= 10 || StartStopSW.PushState == ON_STATE) {
        // キッチンタイマー状態をリセット処理へ変更
        SetKitchenTimerStateToReset();
    }
}

static void reset(void) {
    // キッチンタイマー状態をカウントダウン設定へ変更
    SetKitchenTimerStateToSetting();

    // カウント時間を00m00sへ設定
    SetMinuteCount(0);
    SetSecondCount(0);

    // UpdateLCDフラグをON
    SetUpdateLCDFlgON();

    // タイマレジスタのクリア
    TMR1_Reload();
}