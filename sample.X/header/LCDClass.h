/* 
 * File:   LCDClass.h
 * Author: fuminori.hakoishi
 *
 * Created on 2020/07/16, 14:55
 */

#ifndef LCDCLASS_H
#define	LCDCLASS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"
#include "../mcc_generated_files/examples/i2c1_master_example.h"


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

    void SetPosLCD(uint8_t i_pos);

    void WriteCharToRAM(uint8_t i_char);

    void WriteStringsToRAM(uint8_t * i_str, uint8_t i_len);

#ifdef	__cplusplus
}
#endif

#endif	/* LCDCLASS_H */

