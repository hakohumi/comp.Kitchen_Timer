/*
 * File:   LCDClass.h
 * Author: fuminori.hakoishi
 *
 * Created on 2020/07/16, 14:55
 */

#ifndef LCDCLASS_H
#define LCDCLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../mcc_generated_files/examples/i2c1_master_example.h"
#include "common.h"



void InitLCD(void);

// LCD上の書き込む位置を指定する
inline void SetPosLCD(uint8_t i_pos);  // アドレス指定
void SetPosLineLCD(bool i_row);        // 1行目か2行目の先頭を指定

void WriteCharToRAM(uint8_t i_char);

void Write1LineToLCD(uint8_t *i_str, uint8_t i_len);

// mとsを表示
void SetUnitChar(void);
// mとsをクリア
void ClrUnitChar(void);

// UpdateLCDフラグをONにする
#define SetUpdateLCDFlgON() \
    do {                    \
        UpdateLCDFlg = ON;  \
    } while (0)
#define SetUpdateLCDFlgOFF() \
    do {                     \
        UpdateLCDFlg = OFF;  \
    } while (0)

extern bool UpdateLCDFlg;

// UpdateLCDフラグがONかどうか
bool IsUpdateLCDFlg(void);

static char *utoa(unsigned int value, char *s, int radix);

#ifdef __cplusplus
}
#endif

#endif /* LCDCLASS_H */
