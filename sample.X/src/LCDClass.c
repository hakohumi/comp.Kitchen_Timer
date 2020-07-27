
#include "LCDClass.h"

#include <mcc.h>

static bool IsUpdateLCDFlg = OFF;

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

    // 表示テスト
    uint8_t strBuf1[] = {"hello"};
    uint8_t strBuf2[] = {"world"};

    SetPosLCD(0x00);
    WriteStringsToRAM(strBuf1, 5);
    SetPosLCD(0x40);
    WriteStringsToRAM(strBuf2, 5);
}

void sendCmdLCD(uint8_t i_data) {
    I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, i_data);
}

void SetPosLCD(uint8_t i_pos) {
    // Set DDRAM address DB7 = 1
    // 設定可能ビット DB0 ~ DB6

    I2C1_Write1ByteRegister(LCD_ADDR, CONTROLE_BYTE, (0x80 | i_pos));
}

void WriteCharToRAM(uint8_t i_char) {
    I2C1_Write1ByteRegister(LCD_ADDR, WR_CONTROLE_BYTE, i_char);
}

// i_str は、8文字分の表示させた文字列が入った uint8_t型の配列

void WriteStringsToRAM(uint8_t* i_str, uint8_t i_len) {
    // MAX_BUF_SIZE = 9
    uint8_t l_buf[MAX_BUF_SIZE];
    uint8_t c;

    // もし、8文字より多い文字数が入った場合、
    if (i_len > 8) {
        // 何もしないで抜ける

    } else {
        l_buf[0] = WR_CONTROLE_BYTE;

        for (c = 1; c <= i_len; c++) {
            l_buf[c] = i_str[c - 1];
        }

        I2C1_WriteNBytes(LCD_ADDR, l_buf, ++i_len);
    }
}

void SetUpdateLCDFlgON(void) { IsUpdateLCDFlg = ON; }