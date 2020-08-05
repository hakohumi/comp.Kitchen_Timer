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

inline static void settingCountTime(void);
inline static uint8_t detectSWState(SWState_t *i_SW);
inline static void onGoingCountDown(void);
inline static void endCountDown(void);
inline static void reset(void);

// カウント時間(分)
uint8_t MinuteCountTime = 0;
// カウント時間(秒)
uint8_t SecondCountTime = 0;
// カウントダウン終了カウント
uint8_t CountDownEndCount = 0;

// 1回リセット処理を行うとONになる
static bool resetFlg = OFF;

/*
状態遷移処理
CoutClass
 */

void CountDown(void) {
    //    assert(MinuteCountTime != 0 || SecondCountTime != 0);
    // 秒が0ではないなら、1減少させる
    if (SecondCountTime > 0) {
        SecondCountTime--;
    } else {
        // 分が0ではない、かつ、秒が0の場合、
        // 分を1減少させて、秒を59にする
        MinuteCountTime--;
        SecondCountTime = SECOND_MAX;
    }
}

void StateTransferProcess(void) {
    switch (KitchenTimerState) {
        case COUNTTIME_SETTING_STATE:
            settingCountTime();
            break;
        case COUNTDOWN_ONGOING_STATE:
            onGoingCountDown();
            break;
        case COUNTDOWN_END_STATE:
            endCountDown();
            break;
        case RESET_STATE:
            reset();
            break;
        default:
            // ありえない
            break;
    }
}

// 分カウントを設定
// MINUTE_MAX より上であれば、MINUTE_MAX を格納
// MINUTE_MAX = 99

void SetMinuteCount(uint8_t i_minute) {
    if (MINUTE_MAX < i_minute) {
        MinuteCountTime = MINUTE_MAX;
    } else {
        MinuteCountTime = i_minute;
    }
}

// 秒カウントを設定
// SECOND_MAXより上であれば、SECOND_MAX を格納
// SECOND_MAX = 59

void SetSecondCount(uint8_t i_second) {
    if (SECOND_MAX < i_second) {
        SecondCountTime = SECOND_MAX;
    } else {
        SecondCountTime = i_second;
    }
}

// 引数で指定された値を分カウントに加算する

void AddMinuteCount(uint8_t i_minute) {
    // もし、入力された値が最大値より大きい場合
    // 最大値を設定
    // MINUTE_MAX = 99
    if (i_minute >= MINUTE_MAX) {
        MinuteCountTime = MINUTE_MAX;
    } else {
        /*
         入力が 99 以下の場合

         もし、加算後、分カウントが最大値より高い場合
         最大値より増えないようにする
         */

        MinuteCountTime += i_minute;

        if (MinuteCountTime > MINUTE_MAX) {
            MinuteCountTime = MINUTE_MAX;
        }
    }
}

// 引数で指定された値を秒カウントに加算する

void AddSecondCount(uint8_t i_second) {
    /*
     もし、加算後、秒カウントが最大値より高い場合
     最大値より下回るまで、分カウントに繰り上げる
     */

    // 加算
    SecondCountTime += i_second;

    // 秒が最大値を超えている間
    while (SecondCountTime > SECOND_MAX) {
        // もし、分カウントが最大値だったら
        if (MinuteCountTime == MINUTE_MAX) {
            // 秒カウントは最大値に固定する
            SecondCountTime = SECOND_MAX;

        } else {
            // 分カウントが最大値でなければ、
            // 分カウントに繰り上げる
            AddMinuteCount(1);
            // 秒カウントから、60秒を引く
            SecondCountTime -= (SECOND_MAX + 1);
        }
    }
}

// カウント時間設定

inline static void settingCountTime(void) {
    uint8_t l_addCountTime = 0;

    // 分スイッチ処理
    l_addCountTime = detectSWState(&MinuteSW);
    AddMinuteCount(l_addCountTime);
    // 秒スイッチ処理
    l_addCountTime = detectSWState(&SecondSW);
    AddSecondCount(l_addCountTime);

    // カウント時間が00m00sではない場合、スタートさせない
    if (!(MinuteCountTime == 0 && SecondCountTime == 0)) {
        // スタートストップスイッチ状態はONか
        if (StartStopSW.PushState == ON_STATE) {
            // スタートストップスイッチ状態をOFFにする
            StartStopSW.PushState = OFF_STATE;

            // UpdateLCDフラグをON
            SetUpdateLCDFlg();

            // 0.5秒タイマを起動
            TMR1_StartTimer();

            // キッチンタイマー状態をカウントダウン中へ変更
            SetKitchenTimerStateToGoing();
        }
    }
    // リセットスイッチ状態はONか？
    if (IsPushedResetSW == ON_STATE) {
        // キッチンタイマー状態をリセットへ変更
        SetKitchenTimerStateToReset();
    }
}

inline static uint8_t detectSWState(SWState_t *i_SW) {
    uint8_t l_retval = 0;

    // スイッチのタイミングフラグ
    if (i_SW->TimingFlag == ON) {
        // スイッチの状態は？
        switch (i_SW->PushState) {
            case OFF_STATE:
                // なにもしない
                break;

            // 短押しと長押し1段階目
            case ON_STATE:
            case LONG_STG1_STATE:
                // 1分増やす
                l_retval = 1;

                // UpdateLCDFlg をON
                SetUpdateLCDFlg();
                // タイミングフラグをOFF
                i_SW->TimingFlag = OFF;
                break;
                // 長押し2段階目
            case LONG_STG2_STATE:
                // 10分増やす
                l_retval = 10;

                // UpdateLCDFlg をON
                SetUpdateLCDFlg();

                // タイミングフラグをOFF
                i_SW->TimingFlag = OFF;
                break;
            default:
                /* スルー */

                // 押されていない
                break;
        }
    }
    return l_retval;
}

inline static void onGoingCountDown(void) {
    // スタートストップスイッチ状態はONか
    if (StartStopSW.PushState == ON_STATE) {
        // スタートストップスイッチ状態をOFFにする
        StartStopSW.PushState = OFF_STATE;
        // 0.5秒タイマを停止
        TMR1_StopTimer();

        // 1秒フラグをOFF
        Is1sFlg = OFF;

        // タイマレジスタのクリア
        TMR1_Reload();

        // UpdateLCDフラグをON
        SetUpdateLCDFlg();

        // キッチンタイマー状態をカウント時間設定へ変更
        SetKitchenTimerStateToSetting();
    }

    // リセットスイッチ状態はONか？
    if (IsPushedResetSW == ON_STATE) {
        // 0.5秒タイマを停止
        TMR1_StopTimer();

        // 1秒フラグをOFF
        Is1sFlg = OFF;

        // タイマレジスタのクリア
        TMR1_Reload();

        // キッチンタイマー状態をリセット処理へ変更
        SetKitchenTimerStateToReset();
    }
}

inline static void endCountDown(void) {
    // カウントダウン終了カウントが10以上か
    if (CountDownEndCount >= 10 || StartStopSW.PushState == ON_STATE) {
        // 0.5秒タイマを停止
        TMR1_StopTimer();

        // 1秒フラグをOFF
        Is1sFlg = OFF;

        // タイマレジスタのクリア
        TMR1_Reload();

        // キッチンタイマー状態をリセット処理へ変更
        SetKitchenTimerStateToReset();
    }
}

inline static void reset(void) {
    // 1回だけ処理させる
    if (resetFlg == OFF) {
        // LCDのリセット処理を、このリセット処理が終わってから行うようにするためのフラグ
        SetLCDResetFlg();

        // カウント時間を00m00sへ設定
        SetMinuteCount(0);
        SetSecondCount(0);

        // UpdateLCDフラグをON
        SetUpdateLCDFlg();

        // 1秒フラグをOFF
        Is1sFlg = OFF;

        // タイマレジスタのクリア
        TMR1_Reload();

        // フラグの更新
        resetFlg = ON;
    } else if (resetFlg == ON) {
        // スイッチを押し続けて2回目以降のループ
        // まだ2つのスイッチのいずれかが押され続けている場合

        if (MinuteSW.PushState != OFF_STATE || SecondSW.PushState != OFF_STATE) {
            // 何もしない
        } else {
            /* ボタンが離されたら */

            // リセットフラグをクリア
            resetFlg = OFF_STATE;
            // キッチンタイマー状態をカウントダウン設定へ変更
            SetKitchenTimerStateToSetting();
        }
    }
}