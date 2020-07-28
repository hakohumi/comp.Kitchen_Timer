#include "SW.h"

SWState_t MinuteSWState;
SWState_t SecondSWState;
SWState_t ResetSWState;
SWState_t StartStopSWState;

static SWState_t oldMinuteSWState;
static SWState_t oldSecondSWState;
static SWState_t oldoResetSWState;
static SWState_t oldStartStopSWState;

static void updateAllSWState(SWState_t *i_SWState, SWState_t *i_oldSWState);
static void updateMinuteSWState();
static void updateSecondSWState();
static void updateResetSWState();
static void updateStartStopSWState();

static uint8_t detectEdge(bool i_currentState, bool i_oldState);
static void updateEdgeState(SWState_t *i_SWState, SWState_t *i_oldSWState);

// スイッチのONOFFから、スイッチ状態を判断し、
// スイッチ状態フラグへ格納する

void UpdateSWState() {
    // 分スイッチ の更新
    updateMinuteSWState();
    // 秒スイッチ の更新
    updateSecondSWState();
    // リセットスイッチの更新
    updateResetSWState();
    // スタートストップスイッチ の更新
    updateStartStopSWState();

    // テスト
    //    updateAllSWState(MinuteSWState);
    //    updateAllSWState(SecondSWState);
    //    updateResetSWState();
    //    updateAllSWState(StartStopSWState);
}

static void updateAllSWState(SWState_t *i_SWState, SWState_t *i_oldSWState) {
    // エッジ検知をする
    switch (detectEdge(i_SWState->PushedFlag, i_oldSWState->PushedFlag)) {
        case OFF_STATE:
            // 押されていない場合、状態をクリア
            i_SWState->PushedState = false;
            break;
        case RISING_STATE:
            // 立ち上がり状態の場合
            i_SWState->PushedState = true;
            break;
        case ONGOING_STATE:
            // 継続中状態の場合
            i_SWState->PushedState = true;
            break;
        case FALLING_STATE:
            //立ち下がり状態の場合
            i_SWState->PushedState = false;
            break;
    }
    // エッジの状態を更新する
    updateEdgeState(i_SWState, i_oldSWState);
}


// 分スイッチ状態 更新
// 入力：Flag
// 出力：State

static void updateMinuteSWState() {
    // 押されていたら
    if (MinuteSWState.PushedFlag) {
        // 状態を更新して
        MinuteSWState.PushedState = MinuteSWState.PushedFlag;
        // フラグをクリアする
        MinuteSWState.PushedFlag = false;
    } else {
        // 押されていない場合、状態をクリア
        MinuteSWState.PushedState = false;
    }
}

// 秒スイッチ状態 更新

static void updateSecondSWState() {
    // もし、フラグが立っていたら、
    if (SecondSWState.PushedFlag) {
        // 状態を更新
        SecondSWState.PushedState = SecondSWState.PushedFlag;
        // フラグをクリア
        SecondSWState.PushedFlag = false;
    } else {
        // 押されていなかったら、状態をクリア
        SecondSWState.PushedState = false;
    }
}

// リセットスイッチ状態 更新

static void updateResetSWState() {
    // もし、分スイッチと秒スイッチがどちらも押されていたら
    if (MinuteSWState.PushedState & SecondSWState.PushedState) {
        // どちらの状態もOFFにして、
        MinuteSWState.PushedState = false;
        SecondSWState.PushedState = false;
        // リセットスイッチ状態をONにする
        ResetSWState.PushedState = true;
    } else {
        // 押されていなかったら クリア
        ResetSWState.PushedState = false;
    }
}

// スタートストップスイッチ状態 更新

static void updateStartStopSWState() {
    // 状態を更新する

    StartStopSWState.PushedState = StartStopSWState.PushedFlag;
    // フラグをクリア
    StartStopSWState.PushedFlag = false;
}

bool IsPushedMinuteSW(void) {
    return MinuteSWState.PushedState;
}

bool IsPushedSecondSW(void) {
    return SecondSWState.PushedState;
}

bool IsPushedResetSW(void) {
    return ResetSWState.PushedState;
}

bool IsPushedStartStopSW(void) {
    return StartStopSWState.PushedState;
}

// エッジ検知
// 入力：過去の状態、現在の状態
// 出力：0 = OFF状態, 1 = 立ち上がり状態, 2 = 継続状態, 3 = 立ち下がり状態

static uint8_t detectEdge(bool i_currentState, bool i_oldState) {
    uint8_t l_ret = 0;

    // 前回の状態は？
    if (i_oldState) {
        // 前回の状態は？: YES
        // 現在の状態は？
        if (i_currentState) {
            // 前回の状態は？: YES
            // 現在の状態は？: YES
            // 出力：継続状態
            l_ret = ONGOING_STATE;
        } else {
            // 前回の状態は？: YES
            // 現在の状態は？: NO
            // 出力：立ち下がり状態
            l_ret = FALLING_STATE;
        }
    } else {
        // 前回の状態は？: NO
        // 現在の状態は？
        if (i_currentState) {
            // 前回の状態は？: NO
            // 現在の状態は？: YES
            // 出力：立ち上がり状態
            l_ret = RISING_STATE;
        } else {
            // 前回の状態は？: NO
            // 現在の状態は？: NO
            // 出力：OFF状態
            l_ret = OFF_STATE;
        }
    }
    return l_ret;
}

static void updateEdgeState(SWState_t *i_SWState, SWState_t *i_oldSWState) {
    i_oldSWState->LongPushedState = i_SWState->LongPushedState;
    i_oldSWState->PushedFlag = i_SWState->PushedFlag;
    i_oldSWState->PushedState = i_SWState->PushedState;
}