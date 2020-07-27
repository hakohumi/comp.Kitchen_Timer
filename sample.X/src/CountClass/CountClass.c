#include "CountClass/CountClass.h"

#include "CountClass/endCountDown.h"
#include "CountClass/onGoingCountDown.h"
#include "CountClass/reset.h"
#include "CountClass/settingCountTime.h"

// 分の最大値
#define MINUTE_MAX 99
// 分の最低値
#define MINUTE_MIN 0

// 秒の最大値
#define SECOND_MAX 59
// 秒の最小値
#define SECOND_MIN 0

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
    }
}

// 分カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 99 以外であれば、エラーとして false を返す

bool SetCountMinute(uint8_t i_minute) {
    if ((i_minute < (uint8_t) MINUTE_MIN) || ((uint8_t) MINUTE_MAX < i_minute)) {
        return false;
    }

    MinuteCountTime = i_minute;

    return true;
}

// 秒カウントを設定
// 正常であれば、trueを返す
// もし、0 ~ 59 以外であれば、エラーとして false を返す

bool SetCountSecond(uint8_t i_second) {
    if ((i_second < (uint8_t) SECOND_MIN) || ((uint8_t) SECOND_MAX < i_second)) {
        return false;
    }

    SecondCountTime = i_second;

    return true;
}