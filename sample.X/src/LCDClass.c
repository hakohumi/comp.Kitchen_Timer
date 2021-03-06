
#include "LCDClass.h"

#include <mcc.h>

#include "CountClass.h"
#include "examples/i2c1_master_example.h"

bool UpdateLCDFlg = OFF;
// LCDのリセット処理を、このリセット処理が終わってから行うようにするためのフラグ
bool LCDResetFlg = OFF;

// ---------------------------------------------
// 文字リテラル
// =--------------------------------------------
static uint8_t char_m = 'm';
static uint8_t char_s = 's';
static uint8_t char_blank = ' ';
static uint8_t str_blank[8] = "        ";

// LCDの初期化
// *** ST7032iに対して、書き込みフォーマット ***
// ①まず、7bitのスレーブアドレス(B0111110)と、
// 1bitのR/Wビット（今回、WriteOnlyなのでずっと0)を送信
// B0111110 + 0 = 0x7C
// ②コントロールバイト(8bit)として、
// 1bitの"Co" 連続データ = 1, 最終データ = 0
// 1bitの"RS" "コマンド" = 0, "データ" = 1
// 6bitの"0"(B000000)
// を送信
// ③その後、7bitのデータを送信する

void InitLCD(void) {
    uint8_t l_commandTable[10] = {0x38, 0x39, 0x14, 0x70, 0x52, 0x6C, 0x38, 0x0C, 0x01};
    uint8_t c;

    // 40ms以上待つ
    __delay_ms(40);

    for (c = 0; c < 10; c++) {
        // コマンドを送信
        I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, l_commandTable[c]);
        if (c == 5) {
            __delay_ms(200);
        } else {
            // 26.3us以上待つ
            __delay_us(27);
        }
    }

    // 1.08ms以上 待つ
    __delay_ms(2);
}

void UpdateLCD(void) {
    // UpdateLCDフラグがONか
    if (UpdateLCDFlg == ON) {
        // キッチンタイマーの状態は？
        switch (KitchenTimerState) {
                // カウント時間設定
            case COUNTTIME_SETTING_STATE:

                CountTimeToLCD();

                break;

                // カウントダウン中
            case COUNTDOWN_ONGOING_STATE:
                // カウント時間をLCDバッファに格納
                CountTimeToLCD();

                // 1秒フラグがOFFか
                if (!Is1sFlg) {
                    // LCDバッファのmとsの文字を点滅
                    ClrUnitChar();
                } else {
                    WriteUnitChar();
                }
                break;

                // カウントダウン終了
            case COUNTDOWN_END_STATE:
                // カウント時間をLCDバッファに格納
                CountTimeToLCD();

                // 1秒フラグがOFFか
                if (!Is1sFlg) {
                    // LCDバッファの文字を点滅
                    // ディスプレイをクリアする
                    ClrLineDisplay(LINE_SECOND);
                } else {
                    // カウント時間を表示
                    CountTimeToLCD();
                }

                break;

                // リセット状態
            case RESET_STATE:
                // 1回だけ実行させる
                if (LCDResetFlg == ON) {
                    // ディスプレイをクリアする
                    ClrDisplay();
                    // ClrLineDisplay();
                    // カウント時間を表示
                    CountTimeToLCD();
                    // フラグクリア
                    LCDResetFlg = OFF;
                }
                break;

                // その他の状態 ありえない
            default:

                while (1) {
                    LED1 ^= 1;
                    LED2 ^= 1;
                    LED3 ^= 1;
                    LED4 ^= 1;
                    __delay_ms(500);
                }

                break;
        }
        // LCDバッファの値をLCDへ表示
        // UpdateLCDフラグをOFFにする
        ClrUpdateLCDFlg();
    }
}

// LCD上の書き込む場所を指定

inline void SetPosLCD(uint8_t i_pos) {
    // Set DDRAM address DB7 = 1
    // 設定可能ビット DB0 ~ DB6

    I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, (LCD_SET_POS_DB7 | i_pos));
}

// LCD上の書き込む場所を、
// 上の行か下の行の先頭を指定する
// true だと 2行目
// false だと 1行目

inline void SetPosLineLCD(bool i_row) {
    if (i_row) {
        // true 2行目
        I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, (LCD_SET_POS_DB7 | LINE_SECOND));
    } else {
        // false 1行目
        I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, (LCD_SET_POS_DB7 | LINE_FIRST));
    }
}

// i_str は、8文字分の表示させた文字列が入った uint8_t型の配列

void Write1LineToLCD(uint8_t *i_str, uint8_t i_len) {
    // MAX_BUF_SIZE = 9
    uint8_t l_buf[MAX_BUF_SIZE];
    uint8_t c;

    // もし、8文字より多い文字数が入った場合、
    if (i_len > LINE_DIGITS_MAX) {
        // 何もしないで抜ける

    } else {
        l_buf[0] = WR_CONTROLE_BYTE;

        for (c = 1; c <= i_len; c++) {
            l_buf[c] = i_str[c - 1];
        }

        I2C1_WriteNBytes(LCD_ADDR, l_buf, ++i_len);
    }
}

// カウント時間をLCDへ書き込む
// 2行目の真ん中へ書く

void CountTimeToLCD() {
    uint8_t i_str[8] = "        ";

    // -------------------------------------------------------
    // sprintfは容量が大きいため、後に別の方法で実装する
    // -------------------------------------------------------
    // 分を変換

    // 2桁だったら
    if (MinuteCountTime > 9) {
        i_str[1] = Itochar(MinuteCountTime / 10);
        i_str[2] = Itochar(MinuteCountTime % 10);
    } else {
        // ひと桁だったら
        i_str[1] = '0';
        i_str[2] = Itochar(MinuteCountTime);
    }

    // 秒を変換

    // 2桁だったら
    if (SecondCountTime > 9) {
        i_str[4] = Itochar(SecondCountTime / 10);
        i_str[5] = Itochar(SecondCountTime % 10);
    } else {
        // ひと桁だったら
        i_str[4] = '0';
        i_str[5] = Itochar(SecondCountTime);
    }
    // -------------------------------------------------------

    i_str[3] = char_m;
    i_str[6] = char_s;

    // 書き込み位置を2行目へセット
    SetPosLineLCD(true);
    // 1行書き込む
    Write1LineToLCD(i_str, 8);
}

void WriteUnitChar() {
    // m表示
    SetPosLCD(LINE_SECOND + 3);
    I2C1_Write1ByteRegister(LCD_ADDR, WR_CONTROLE_BYTE, char_m);

    // S表示
    SetPosLCD(LINE_SECOND + 6);
    I2C1_Write1ByteRegister(LCD_ADDR, WR_CONTROLE_BYTE, char_s);
}

void ClrUnitChar() {
    // m削除
    SetPosLCD(LINE_SECOND + 3);
    I2C1_Write1ByteRegister(LCD_ADDR, WR_CONTROLE_BYTE, char_blank);

    // S表示
    SetPosLCD(LINE_SECOND + 6);
    I2C1_Write1ByteRegister(LCD_ADDR, WR_CONTROLE_BYTE, char_blank);
}

// ClearDisplay

void ClrLineDisplay(uint8_t i_line) {
    SetPosLCD(i_line);
    Write1LineToLCD(str_blank, 8);
}

void ClrDisplay(void) {
    I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, CMD_LCD_CLR_DISPLAY);
}

inline void SetLCDResetFlg(void) {
    LCDResetFlg = ON;
}

// inline void ClrLCDResetFlg(void) { LCDResetFLg = OFF; }

// 数値一文字をchar型へ変換
uint8_t Itochar(uint8_t value) {
    if (value > DECIMAL_MAX) {
        return char_blank;
    }
    return "0123456789"[value];
}

#ifdef DEBUG

char *utoa(unsigned int value, char *s, int radix) {
    char *s1 = s;
    char *s2 = s;

    do {
        *s2++ = "0123456789abcdefghijklmnopqrstuvwxyz"[value % radix];
        value /= radix;
    } while (value > 0);

    *s2-- = '\0';

    while (s1 < s2) {
        char c = *s1;
        *s1++ = *s2;
        *s2-- = c;
    }
    return s;
}

// Display ON
void DisplayON(void) {
    I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, CMD_LCD_DISPLAY_ON);
}
// Display OFF
void DisplayOFF(void) {
    I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, CMD_LCD_DISPLAY_OFF);
}
#endif