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

// (B0111110);
#define LCD_ADDR (i2c1_address_t)(0x3E)
// Co = 0, RS = 0, Control byte = 0;
#define CONTROLE_BYTE (uint8_t)(0x00)
// RSビットが立っているとき
#define WR_CONTROLE_BYTE (uint8_t)(0x40)
// 1つコマンド 8文字表示
#define MAX_BUF_SIZE 9

void InitLCD(void);

void sendCmdLCD(uint8_t i_data);

// LCD上の書き込む位置を指定する
void SetPosLCD(uint8_t i_pos);   // アドレス指定
void SetPosLineLCD(bool i_row);  // 1行目か2行目の先頭を指定

void WriteCharToRAM(uint8_t i_char);

void Write1LineToRAM(uint8_t* i_str, uint8_t i_len);

// UpdateLCDフラグをONにする
void SetUpdateLCDFlgON(void);

// UpdateLCDフラグがONかどうか
bool IsUpdateLCDFlg(void);

#ifdef __cplusplus
}
#endif

#endif /* LCDCLASS_H */
