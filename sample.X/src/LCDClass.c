
#include "LCDClass.h"

#include <mcc.h>

#define LINE_1 0x00
#define LINE_2 0x40
#define LINE_DIGITS_MAX 8
#define LCD_SET_POS_DB7 0x80

bool UpdateLCDFlg = OFF;

// ---------------------------------------------
// 文字リテラル
// =--------------------------------------------
static uint8_t Str_SETTING[] = "fafa";
static uint8_t Str_m = 'm';
static uint8_t Str_s = 's';

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
    uint8_t l_commandTable[10] = {0x38, 0x39, 0x14, 0x70, 0x52,
                                  0x6C, 0x38, 0x0C, 0x01};
    uint8_t c;

    // 40ms以上待つ
    __delay_ms(40);

    for (c = 0; c < 10; c++) {
        sendCmdLCD(l_commandTable[c]);

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

void sendCmdLCD(uint8_t i_data) {
    I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, i_data);
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

void SetPosLineLCD(bool i_row) {
    if (i_row) {
        // true 2行目
        SetPosLCD(LINE_2);
    } else {
        // false 1行目
        SetPosLCD(LINE_1);
    }
}

void WriteCharToRAM(uint8_t i_char) {
    I2C1_Write1ByteRegister(LCD_ADDR, WR_CONTROLE_BYTE, i_char);
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

void SetUnitChar() {
    // m表示
    SetPosLCD(LINE_2 + 3);
    WriteCharToRAM('m');

    // S表示
    SetPosLCD(LINE_2 + 6);
    WriteCharToRAM('s');
}

void ClrUnitChar() {
    // m削除
    SetPosLCD(LINE_2 + 3);
    WriteCharToRAM(' ');

    // S表示
    SetPosLCD(LINE_2 + 6);
    WriteCharToRAM(' ');
}

bool IsUpdateLCDFlg(void) { return (UpdateLCDFlg); }

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